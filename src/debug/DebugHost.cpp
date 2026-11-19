// OpenMSE.DebugHost: managed (CLR v2) debug adapter built on ICorDebug and the PDB symbol reader.
// One process debugs one program. Protocol: UTF-8 JSON lines on stdin/stdout.
//   request  {"id":"1","method":"launch","params":{...}}   response {"id":"1","ok":true,"result":{...}}
//   event    {"type":"event","event":"stopped|output|exited|breakpoint|module","body":{...}}
// Built twice (x86 and x64): ICorDebug only debugs processes of its own bitness.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#pragma warning(push)
#pragma warning(disable : 4091)
#include <cor.h>
#include <cordebug.h>
#include <corsym.h>
#pragma warning(pop)
#include <wrl/client.h>

#include "../core/json.hpp"
#include "../core/text.hpp"

#include <algorithm>
#include <cctype>
#include <condition_variable>
#include <cstdio>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

#pragma comment(lib, "corguids.lib")
#pragma comment(lib, "ole32.lib")

using Microsoft::WRL::ComPtr;
using namespace openmse::core;

namespace {

constexpr ULONG32 hidden_line = 0xFEEFEE;

std::mutex out_lock;
void emit(const std::string& line)
{
    std::lock_guard<std::mutex> hold(out_lock);
    std::fwrite(line.data(), 1, line.size(), stdout);
    std::fputc('\n', stdout);
    std::fflush(stdout);
}
void event(const char* name, const std::string& body) { emit(std::string("{\"type\":\"event\",\"event\":\"") + name + "\",\"body\":" + body + "}"); }

struct Failure { std::string code, message; };
[[noreturn]] void fail(const std::string& code, const std::string& message) { throw Failure{ code, message }; }
void check(HRESULT hr, const char* what)
{
    if (FAILED(hr)) {
        char buf[16];
        std::snprintf(buf, sizeof(buf), "0x%08lX", static_cast<unsigned long>(hr));
        fail("debugger_error", std::string(what) + " 失败（" + buf + "）");
    }
}

std::string u8(const wchar_t* w) { try { return narrow(w ? w : L""); } catch (...) { return "?"; } }
bool same_file(const std::wstring& a, const std::wstring& b) { return CompareStringOrdinal(a.c_str(), -1, b.c_str(), -1, TRUE) == CSTR_EQUAL; }

struct ModuleInfo {
    ComPtr<ICorDebugModule> module;
    ComPtr<IMetaDataImport> metadata;
    ComPtr<ISymUnmanagedReader> symbols;
    std::wstring path;
};

struct SourceBreakpoint {
    int id = 0;
    std::wstring file;
    ULONG32 line = 0, bound_line = 0;
    bool verified = false;
    std::vector<ComPtr<ICorDebugFunctionBreakpoint>> bound;
};

struct Location {
    std::string function, file;
    ULONG32 line = 0, column = 0, il_offset = 0;
    bool has_source = false;
};

class Debugger;
Debugger* g_debugger = nullptr;

class Debugger : public ICorDebugManagedCallback, public ICorDebugManagedCallback2 {
public:
    std::recursive_mutex lock;
    ComPtr<ICorDebug> cordebug;
    ComPtr<ICorDebugProcess> process;
    ComPtr<ICorDebugThread> stopped_thread;
    ComPtr<ICorDebugStepper> active_stepper;
    ComPtr<ISymUnmanagedBinder> binder;
    std::map<CORDB_ADDRESS, ModuleInfo> modules;
    std::vector<SourceBreakpoint> breakpoints;
    int next_breakpoint = 1;
    bool stopped = false, exited = false, step_pending = false, step_in = false;
    int auto_steps = 0;
    HANDLE output_read = nullptr;
    std::thread output_thread;

    // ---- IUnknown: the callback object lives for the whole process.
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override
    {
        if (riid == IID_IUnknown || riid == IID_ICorDebugManagedCallback) { *out = static_cast<ICorDebugManagedCallback*>(this); return S_OK; }
        if (riid == IID_ICorDebugManagedCallback2) { *out = static_cast<ICorDebugManagedCallback2*>(this); return S_OK; }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return 1; }
    ULONG STDMETHODCALLTYPE Release() override { return 1; }

    // ---- metadata and symbols
    ModuleInfo* info(ICorDebugModule* module)
    {
        CORDB_ADDRESS base = 0;
        if (!module || FAILED(module->GetBaseAddress(&base))) return nullptr;
        auto it = modules.find(base);
        return it == modules.end() ? nullptr : &it->second;
    }
    std::string method_name(ModuleInfo* m, mdMethodDef token)
    {
        if (!m || !m->metadata) return "?";
        wchar_t name[512] = L"", type[512] = L"";
        mdTypeDef owner = 0;
        ULONG n = 0;
        if (FAILED(m->metadata->GetMethodProps(token, &owner, name, 512, &n, nullptr, nullptr, nullptr, nullptr, nullptr))) return "?";
        if (owner != mdTypeDefNil) m->metadata->GetTypeDefProps(owner, type, 512, &n, nullptr, nullptr);
        return u8(type) + (type[0] ? "." : "") + u8(name);
    }
    std::string type_name(ModuleInfo* m, mdTypeDef token)
    {
        wchar_t type[512] = L"";
        ULONG n = 0;
        if (m && m->metadata) m->metadata->GetTypeDefProps(token, type, 512, &n, nullptr, nullptr);
        return u8(type);
    }

    struct Points { std::vector<ULONG32> offsets, lines, columns; std::vector<std::wstring> files; };
    bool sequence_points(ModuleInfo* m, mdMethodDef token, Points& p)
    {
        if (!m || !m->symbols) return false;
        ComPtr<ISymUnmanagedMethod> method;
        if (FAILED(m->symbols->GetMethod(token, &method))) return false;
        ULONG32 count = 0;
        if (FAILED(method->GetSequencePointCount(&count)) || !count) return false;
        std::vector<ISymUnmanagedDocument*> docs(count);
        std::vector<ULONG32> end_lines(count), end_columns(count);
        p.offsets.resize(count); p.lines.resize(count); p.columns.resize(count);
        if (FAILED(method->GetSequencePoints(count, &count, p.offsets.data(), docs.data(), p.lines.data(), p.columns.data(), end_lines.data(), end_columns.data()))) return false;
        for (ULONG32 i = 0; i < count; ++i) {
            wchar_t url[1024] = L"";
            ULONG32 len = 0;
            if (docs[i]) { docs[i]->GetURL(1024, &len, url); docs[i]->Release(); }
            p.files.push_back(url);
        }
        return true;
    }

    Location locate(ICorDebugFrame* frame)
    {
        Location l;
        ComPtr<ICorDebugFunction> function;
        ComPtr<ICorDebugModule> module;
        mdMethodDef token = 0;
        if (FAILED(frame->GetFunction(&function)) || FAILED(function->GetModule(&module)) || FAILED(function->GetToken(&token))) { l.function = "[外部代码]"; return l; }
        ModuleInfo* m = info(module.Get());
        l.function = method_name(m, token);
        ComPtr<ICorDebugILFrame> il;
        if (FAILED(frame->QueryInterface(IID_ICorDebugILFrame, &il))) return l;
        CorDebugMappingResult mapping;
        if (FAILED(il->GetIP(&l.il_offset, &mapping))) return l;
        Points p;
        if (!sequence_points(m, token, p)) return l;
        int best = -1;
        for (std::size_t i = 0; i < p.offsets.size(); ++i)
            if (p.offsets[i] <= l.il_offset && p.lines[i] != hidden_line && (best < 0 || p.offsets[i] >= p.offsets[best])) best = static_cast<int>(i);
        if (best >= 0) {
            std::wstring file = p.files[best];
            if (file.size() > 1 && file[1] == L':') file[0] = towupper(file[0]); // PDBs often store a lower-case drive
            l.has_source = true; l.file = u8(file.c_str()); l.line = p.lines[best]; l.column = p.columns[best];
        }
        if (best >= 0) {
            // At a hidden sequence point the location is approximate; report it as such.
            for (std::size_t i = 0; i < p.offsets.size(); ++i) if (p.offsets[i] == l.il_offset && p.lines[i] == hidden_line) l.has_source = false;
        }
        return l;
    }

    std::vector<ComPtr<ICorDebugFrame>> frames(ICorDebugThread* thread)
    {
        std::vector<ComPtr<ICorDebugFrame>> out;
        ComPtr<ICorDebugChainEnum> chains;
        if (FAILED(thread->EnumerateChains(&chains))) return out;
        ICorDebugChain* chain = nullptr;
        ULONG got = 0;
        while (out.size() < 64 && chains->Next(1, &chain, &got) == S_OK && got == 1) {
            ComPtr<ICorDebugChain> c;
            c.Attach(chain);
            BOOL managed = FALSE;
            c->IsManaged(&managed);
            if (!managed) continue;
            ComPtr<ICorDebugFrameEnum> list;
            if (FAILED(c->EnumerateFrames(&list))) continue;
            ICorDebugFrame* f = nullptr;
            ULONG n = 0;
            while (out.size() < 64 && list->Next(1, &f, &n) == S_OK && n == 1) { ComPtr<ICorDebugFrame> frame; frame.Attach(f); out.push_back(frame); }
        }
        return out;
    }

    std::string stack_json(ICorDebugThread* thread)
    {
        std::string j = "[";
        auto list = frames(thread);
        for (std::size_t i = 0; i < list.size(); ++i) {
            Location l = locate(list[i].Get());
            j += (i ? "," : "") + std::string("{\"index\":") + std::to_string(i) + ",\"function\":" + json_quote(l.function)
                + ",\"file\":" + (l.has_source ? json_quote(l.file) : "null") + ",\"line\":" + std::to_string(l.has_source ? l.line : 0)
                + ",\"column\":" + std::to_string(l.has_source ? l.column : 0) + ",\"il_offset\":" + std::to_string(l.il_offset) + "}";
        }
        return j + "]";
    }

    // ---- breakpoints
    void bind(SourceBreakpoint& bp, ModuleInfo& m)
    {
        if (!m.symbols) return;
        ULONG32 count = 0;
        if (FAILED(m.symbols->GetDocuments(0, &count, nullptr)) || !count) return;
        std::vector<ISymUnmanagedDocument*> docs(count);
        m.symbols->GetDocuments(count, &count, docs.data());
        for (ULONG32 d = 0; d < count; ++d) {
            ComPtr<ISymUnmanagedDocument> doc;
            doc.Attach(docs[d]);
            wchar_t url[1024] = L"";
            ULONG32 len = 0;
            doc->GetURL(1024, &len, url);
            if (!same_file(url, bp.file)) continue;
            // Bind to the first line at or after the requested one that has a sequence point.
            for (ULONG32 line = bp.line; line < bp.line + 30 && !bp.verified; ++line) {
                ComPtr<ISymUnmanagedMethod> method;
                if (FAILED(m.symbols->GetMethodFromDocumentPosition(doc.Get(), line, 0, &method))) continue;
                ULONG32 offset = 0;
                if (FAILED(method->GetOffset(doc.Get(), line, 0, &offset))) continue;
                mdMethodDef token = 0;
                method->GetToken(reinterpret_cast<mdToken*>(&token));
                ComPtr<ICorDebugFunction> function;
                ComPtr<ICorDebugCode> code;
                ComPtr<ICorDebugFunctionBreakpoint> created;
                if (FAILED(m.module->GetFunctionFromToken(token, &function)) || FAILED(function->GetILCode(&code)) || FAILED(code->CreateBreakpoint(offset, &created))) continue;
                created->Activate(TRUE);
                bp.bound.push_back(created);
                bp.verified = true;
                bp.bound_line = line;
            }
        }
        if (bp.verified) event("breakpoint", breakpoint_json(bp));
    }
    std::string breakpoint_json(const SourceBreakpoint& bp)
    {
        return "{\"id\":" + std::to_string(bp.id) + ",\"file\":" + json_quote(u8(bp.file.c_str())) + ",\"line\":" + std::to_string(bp.line)
            + ",\"verified\":" + (bp.verified ? "true" : "false") + ",\"bound_line\":" + std::to_string(bp.bound_line) + "}";
    }

    // ---- values
    std::string element_name(CorElementType t)
    {
        switch (t) {
        case ELEMENT_TYPE_BOOLEAN: return "bool"; case ELEMENT_TYPE_CHAR: return "char"; case ELEMENT_TYPE_I1: return "sbyte";
        case ELEMENT_TYPE_U1: return "byte"; case ELEMENT_TYPE_I2: return "short"; case ELEMENT_TYPE_U2: return "ushort";
        case ELEMENT_TYPE_I4: return "int"; case ELEMENT_TYPE_U4: return "uint"; case ELEMENT_TYPE_I8: return "long";
        case ELEMENT_TYPE_U8: return "ulong"; case ELEMENT_TYPE_R4: return "float"; case ELEMENT_TYPE_R8: return "double";
        case ELEMENT_TYPE_I: return "IntPtr"; case ELEMENT_TYPE_U: return "UIntPtr"; case ELEMENT_TYPE_STRING: return "string";
        default: return "object";
        }
    }
    // Formats a value; `type` receives a type name. Objects show their class name only (no implicit function evaluation).
    std::string format(ICorDebugValue* value, std::string& type, int depth = 0)
    {
        CorElementType t;
        if (FAILED(value->GetType(&t))) { type = "?"; return "?"; }
        type = element_name(t);
        ComPtr<ICorDebugGenericValue> generic;
        if (t <= ELEMENT_TYPE_R8 && t != ELEMENT_TYPE_STRING && SUCCEEDED(value->QueryInterface(IID_ICorDebugGenericValue, &generic))) {
            unsigned char b[8] = {};
            generic->GetValue(b);
            char text[64];
            switch (t) {
            case ELEMENT_TYPE_BOOLEAN: return b[0] ? "true" : "false";
            case ELEMENT_TYPE_CHAR: { wchar_t c[2] = { *reinterpret_cast<wchar_t*>(b), 0 }; return "'" + u8(c) + "'"; }
            case ELEMENT_TYPE_I1: std::snprintf(text, 64, "%d", *reinterpret_cast<signed char*>(b)); break;
            case ELEMENT_TYPE_U1: std::snprintf(text, 64, "%u", b[0]); break;
            case ELEMENT_TYPE_I2: std::snprintf(text, 64, "%d", *reinterpret_cast<short*>(b)); break;
            case ELEMENT_TYPE_U2: std::snprintf(text, 64, "%u", *reinterpret_cast<unsigned short*>(b)); break;
            case ELEMENT_TYPE_I4: std::snprintf(text, 64, "%d", *reinterpret_cast<int*>(b)); break;
            case ELEMENT_TYPE_U4: std::snprintf(text, 64, "%u", *reinterpret_cast<unsigned*>(b)); break;
            case ELEMENT_TYPE_I8: std::snprintf(text, 64, "%lld", *reinterpret_cast<long long*>(b)); break;
            case ELEMENT_TYPE_U8: std::snprintf(text, 64, "%llu", *reinterpret_cast<unsigned long long*>(b)); break;
            case ELEMENT_TYPE_R4: std::snprintf(text, 64, "%g", *reinterpret_cast<float*>(b)); break;
            case ELEMENT_TYPE_R8: std::snprintf(text, 64, "%g", *reinterpret_cast<double*>(b)); break;
            default: return "?";
            }
            return text;
        }
        ComPtr<ICorDebugReferenceValue> reference;
        if (SUCCEEDED(value->QueryInterface(IID_ICorDebugReferenceValue, &reference))) {
            BOOL null = FALSE;
            reference->IsNull(&null);
            if (null) return "null";
            ComPtr<ICorDebugValue> target;
            if (FAILED(reference->Dereference(&target)) || depth > 3) return "?";
            std::string inner_type;
            std::string text = format(target.Get(), inner_type, depth + 1);
            if (t != ELEMENT_TYPE_STRING) type = inner_type;
            return text;
        }
        ComPtr<ICorDebugStringValue> str;
        if (SUCCEEDED(value->QueryInterface(IID_ICorDebugStringValue, &str))) {
            ULONG32 length = 0, got = 0;
            str->GetLength(&length);
            std::wstring w(std::min<ULONG32>(length, 4096) + 1, L'\0');
            str->GetString(static_cast<ULONG32>(w.size()), &got, w.data());
            w.resize(std::min<ULONG32>(got, std::min<ULONG32>(length, 4096)));
            type = "string";
            return json_quote(u8(w.c_str()));
        }
        ComPtr<ICorDebugBoxValue> box;
        if (SUCCEEDED(value->QueryInterface(IID_ICorDebugBoxValue, &box))) {
            ComPtr<ICorDebugObjectValue> object;
            if (SUCCEEDED(box->GetObject(&object))) return format(object.Get(), type, depth + 1);
        }
        ComPtr<ICorDebugArrayValue> array;
        if (SUCCEEDED(value->QueryInterface(IID_ICorDebugArrayValue, &array))) {
            ULONG32 count = 0;
            array->GetCount(&count);
            type = "array";
            return "{长度 " + std::to_string(count) + " 的数组}";
        }
        ComPtr<ICorDebugObjectValue> object;
        if (SUCCEEDED(value->QueryInterface(IID_ICorDebugObjectValue, &object))) {
            ComPtr<ICorDebugClass> cls;
            ComPtr<ICorDebugModule> module;
            mdTypeDef token = 0;
            if (SUCCEEDED(object->GetClass(&cls)) && SUCCEEDED(cls->GetModule(&module)) && SUCCEEDED(cls->GetToken(&token))) {
                type = type_name(info(module.Get()), token);
                return "{" + type + "}";
            }
        }
        return "?";
    }

    struct Variable { std::string name, kind; ComPtr<ICorDebugValue> value; };
    void collect_locals(ISymUnmanagedScope* scope, ULONG32 ip, ICorDebugILFrame* frame, std::vector<Variable>& out, std::set<std::string>& seen)
    {
        ULONG32 start = 0, end = 0;
        scope->GetStartOffset(&start);
        scope->GetEndOffset(&end);
        if (ip < start || ip >= end) return;
        ULONG32 count = 0;
        scope->GetLocals(0, &count, nullptr);
        std::vector<ISymUnmanagedVariable*> locals(count);
        if (count) scope->GetLocals(count, &count, locals.data());
        for (auto* raw : locals) {
            ComPtr<ISymUnmanagedVariable> v;
            v.Attach(raw);
            wchar_t name[256] = L"";
            ULONG32 len = 0, slot = 0;
            v->GetName(256, &len, name);
            v->GetAddressField1(&slot);
            std::string n = u8(name);
            if (n.empty() || n.rfind("CS$", 0) == 0 || n.rfind("VB$", 0) == 0 || !seen.insert(n).second) continue; // compiler temporaries
            ComPtr<ICorDebugValue> value;
            if (SUCCEEDED(frame->GetLocalVariable(slot, &value))) out.push_back({ n, "local", value });
        }
        ULONG32 children = 0;
        scope->GetChildren(0, &children, nullptr);
        std::vector<ISymUnmanagedScope*> list(children);
        if (children) scope->GetChildren(children, &children, list.data());
        for (auto* raw : list) { ComPtr<ISymUnmanagedScope> c; c.Attach(raw); collect_locals(c.Get(), ip, frame, out, seen); }
    }

    std::vector<Variable> variables(std::size_t index)
    {
        if (!stopped || !stopped_thread) fail("invalid_state", "调试目标未处于暂停状态。");
        auto list = frames(stopped_thread.Get());
        if (index >= list.size()) fail("invalid_argument", "栈帧不存在。");
        ComPtr<ICorDebugILFrame> il;
        if (FAILED(list[index]->QueryInterface(IID_ICorDebugILFrame, &il))) fail("unsupported", "该栈帧不是托管 IL 帧。");
        ComPtr<ICorDebugFunction> function;
        ComPtr<ICorDebugModule> module;
        mdMethodDef token = 0;
        check(list[index]->GetFunction(&function), "GetFunction");
        function->GetModule(&module);
        function->GetToken(&token);
        ModuleInfo* m = info(module.Get());
        std::vector<Variable> out;
        std::set<std::string> seen;
        // Arguments: names from metadata; slot 0 is "this" for instance methods.
        if (m && m->metadata) {
            DWORD attributes = 0;
            mdTypeDef owner = 0;
            ULONG n = 0;
            m->metadata->GetMethodProps(token, &owner, nullptr, 0, &n, &attributes, nullptr, nullptr, nullptr, nullptr);
            bool is_static = (attributes & mdStatic) != 0;
            if (!is_static) { ComPtr<ICorDebugValue> self; if (SUCCEEDED(il->GetArgument(0, &self))) { out.push_back({ "this", "argument", self }); seen.insert("this"); } }
            HCORENUM e = nullptr;
            mdParamDef params[64];
            ULONG got = 0;
            m->metadata->EnumParams(&e, token, params, 64, &got);
            m->metadata->CloseEnum(e);
            for (ULONG i = 0; i < got; ++i) {
                mdMethodDef method = 0;
                ULONG sequence = 0, len = 0;
                wchar_t name[256] = L"";
                if (FAILED(m->metadata->GetParamProps(params[i], &method, &sequence, name, 256, &len, nullptr, nullptr, nullptr, nullptr)) || sequence == 0) continue;
                ComPtr<ICorDebugValue> value;
                if (SUCCEEDED(il->GetArgument(sequence - 1 + (is_static ? 0 : 1), &value)) && seen.insert(u8(name)).second) out.push_back({ u8(name), "argument", value });
            }
        }
        if (m && m->symbols) {
            ComPtr<ISymUnmanagedMethod> method;
            ComPtr<ISymUnmanagedScope> root;
            ULONG32 ip = 0;
            CorDebugMappingResult mapping;
            il->GetIP(&ip, &mapping);
            if (SUCCEEDED(m->symbols->GetMethod(token, &method)) && SUCCEEDED(method->GetRootScope(&root))) collect_locals(root.Get(), ip, il.Get(), out, seen);
        }
        return out;
    }

    // name(.field)* over locals/arguments; no function evaluation, so inspecting never runs debuggee code.
    std::string evaluate(std::size_t index, const std::string& expression, std::string& type)
    {
        std::vector<std::string> parts;
        std::size_t start = 0;
        for (std::size_t i = 0; i <= expression.size(); ++i) {
            if (i == expression.size() || expression[i] == '.') {
                std::string p = expression.substr(start, i - start);
                p.erase(0, p.find_first_not_of(" \t"));
                p.erase(p.find_last_not_of(" \t") + 1);
                if (p.empty() || !(std::isalpha(static_cast<unsigned char>(p[0])) || p[0] == '_')) fail("unsupported_expression", "只支持变量名及其字段访问（例如 answer 或 this.count）。");
                for (unsigned char c : p) if (!(std::isalnum(c) || c == '_')) fail("unsupported_expression", "只支持变量名及其字段访问（例如 answer 或 this.count）。");
                parts.push_back(p);
                start = i + 1;
            }
        }
        auto vars = variables(index);
        ComPtr<ICorDebugValue> current;
        for (auto& v : vars) if (v.name == parts[0]) current = v.value;
        if (!current) fail("not_found", "当前栈帧中没有名为 " + parts[0] + " 的变量或参数。");
        for (std::size_t i = 1; i < parts.size(); ++i) {
            ComPtr<ICorDebugReferenceValue> reference;
            if (SUCCEEDED(current->QueryInterface(IID_ICorDebugReferenceValue, &reference))) {
                BOOL null = FALSE;
                reference->IsNull(&null);
                if (null) fail("null_reference", parts[i - 1] + " 为 null。");
                ComPtr<ICorDebugValue> target;
                check(reference->Dereference(&target), "Dereference");
                current = target;
            }
            ComPtr<ICorDebugObjectValue> object;
            if (FAILED(current->QueryInterface(IID_ICorDebugObjectValue, &object))) fail("not_found", parts[i - 1] + " 没有字段。");
            ComPtr<ICorDebugClass> cls;
            ComPtr<ICorDebugModule> module;
            mdTypeDef owner = 0;
            object->GetClass(&cls);
            cls->GetModule(&module);
            cls->GetToken(&owner);
            ModuleInfo* m = info(module.Get());
            mdFieldDef field = 0;
            if (!m || !m->metadata || FAILED(m->metadata->FindField(owner, widen(parts[i]).c_str(), nullptr, 0, &field))) fail("not_found", "类型中没有字段 " + parts[i] + "（不查找基类与属性）。");
            ComPtr<ICorDebugValue> next;
            check(object->GetFieldValue(cls.Get(), field, &next), "GetFieldValue");
            current = next;
        }
        return format(current.Get(), type);
    }

    // ---- execution control
    void resume()
    {
        stopped = false;
        stopped_thread.Reset();
        check(process->Continue(FALSE), "Continue");
    }
    void step(const std::string& kind)
    {
        if (!stopped || !stopped_thread) fail("invalid_state", "调试目标未处于暂停状态。");
        ComPtr<ICorDebugStepper> stepper;
        check(stopped_thread->CreateStepper(&stepper), "CreateStepper");
        stepper->SetUnmappedStopMask(STOP_NONE);
        stepper->SetInterceptMask(INTERCEPT_NONE);
        ComPtr<ICorDebugStepper2> stepper2;
        if (SUCCEEDED(stepper.As(&stepper2))) stepper2->SetJMC(TRUE); // only stop in code with symbols
        if (kind == "out") check(stepper->StepOut(), "StepOut");
        else {
            bool into = kind == "into";
            ComPtr<ICorDebugFrame> frame;
            check(stopped_thread->GetActiveFrame(&frame), "GetActiveFrame");
            ComPtr<ICorDebugFunction> function;
            ComPtr<ICorDebugModule> module;
            ComPtr<ICorDebugILFrame> il;
            ComPtr<ICorDebugCode> code;
            mdMethodDef token = 0;
            ULONG32 ip = 0, size = 0;
            CorDebugMappingResult mapping;
            frame->GetFunction(&function);
            function->GetModule(&module);
            function->GetToken(&token);
            frame->QueryInterface(IID_ICorDebugILFrame, &il);
            if (il) il->GetIP(&ip, &mapping);
            function->GetILCode(&code);
            if (code) code->GetSize(&size);
            Points p;
            if (sequence_points(info(module.Get()), token, p)) {
                // Step over the whole source line: from its sequence point to the next one.
                COR_DEBUG_STEP_RANGE range{ 0, size };
                for (std::size_t i = 0; i < p.offsets.size(); ++i) {
                    if (p.offsets[i] <= ip) range.startOffset = p.offsets[i];
                    else { range.endOffset = p.offsets[i]; break; }
                }
                check(stepper->StepRange(into, &range, 1), "StepRange");
            } else check(stepper->Step(into), "Step");
            step_in = into;
        }
        step_pending = true;
        auto_steps = 0;
        active_stepper = stepper;
        resume();
    }

    void stop_at(ICorDebugAppDomain*, ICorDebugThread* thread, const std::string& reason, const std::string& extra)
    {
        // A breakpoint, pause or exception ends any unfinished step, as in Visual Studio.
        if (active_stepper && reason != "step") active_stepper->Deactivate();
        active_stepper.Reset();
        stopped = true;
        stopped_thread = thread;
        event("stopped", "{\"reason\":\"" + reason + "\"" + extra + ",\"frames\":" + stack_json(thread) + "}");
    }

    // ---- ICorDebugManagedCallback
    HRESULT STDMETHODCALLTYPE Breakpoint(ICorDebugAppDomain* ad, ICorDebugThread* thread, ICorDebugBreakpoint* hit) override
    {
        std::lock_guard<std::recursive_mutex> hold(lock);
        int id = 0;
        for (auto& bp : breakpoints) for (auto& b : bp.bound) if (static_cast<ICorDebugBreakpoint*>(b.Get()) == hit) id = bp.id;
        ComPtr<ICorDebugFunctionBreakpoint> fbp;
        if (!id && SUCCEEDED(hit->QueryInterface(IID_ICorDebugFunctionBreakpoint, &fbp)))
            for (auto& bp : breakpoints) for (auto& b : bp.bound) if (b.Get() == fbp.Get()) id = bp.id;
        step_pending = false;
        stop_at(ad, thread, "breakpoint", ",\"breakpoint\":" + std::to_string(id));
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE StepComplete(ICorDebugAppDomain* ad, ICorDebugThread* thread, ICorDebugStepper*, CorDebugStepReason) override
    {
        std::lock_guard<std::recursive_mutex> hold(lock);
        step_pending = false;
        ComPtr<ICorDebugFrame> frame;
        if (SUCCEEDED(thread->GetActiveFrame(&frame)) && frame && auto_steps < 10) {
            Location l = locate(frame.Get());
            if (!l.has_source) {
                // Landed on a hidden sequence point (compiler-generated code): keep stepping to real source.
                ++auto_steps;
                stopped = true;
                stopped_thread = thread;
                try { int steps = auto_steps; step(step_in ? "into" : "over"); auto_steps = steps; return S_OK; } catch (...) {}
            }
        }
        stop_at(ad, thread, "step", "");
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE Break(ICorDebugAppDomain* ad, ICorDebugThread* thread) override
    {
        std::lock_guard<std::recursive_mutex> hold(lock);
        stop_at(ad, thread, "pause", "");
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE Exception(ICorDebugAppDomain* ad, ICorDebugThread* thread, BOOL unhandled) override
    {
        std::lock_guard<std::recursive_mutex> hold(lock);
        if (unhandled) { stop_at(ad, thread, "exception", ",\"unhandled\":true"); return S_OK; }
        return ad->Continue(FALSE);
    }
    HRESULT STDMETHODCALLTYPE EvalComplete(ICorDebugAppDomain* ad, ICorDebugThread*, ICorDebugEval*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE EvalException(ICorDebugAppDomain* ad, ICorDebugThread*, ICorDebugEval*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE CreateProcess(ICorDebugProcess* p) override { return p->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE ExitProcess(ICorDebugProcess* p) override
    {
        std::lock_guard<std::recursive_mutex> hold(lock);
        DWORD code = 0;
        HPROCESS handle = nullptr;
        if (SUCCEEDED(p->GetHandle(&handle))) GetExitCodeProcess(handle, &code);
        exited = true;
        stopped = false;
        if (output_thread.joinable()) output_thread.join(); // deliver all output before the exit event
        // Release symbol readers now: they keep the .pdb files open, which would block the next build.
        for (auto& bp : breakpoints) bp.bound.clear();
        stopped_thread.Reset();
        active_stepper.Reset();
        modules.clear();
        binder.Reset();
        event("exited", "{\"exit_code\":" + std::to_string(static_cast<long>(code)) + "}");
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE CreateThread(ICorDebugAppDomain* ad, ICorDebugThread*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE ExitThread(ICorDebugAppDomain* ad, ICorDebugThread*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE LoadModule(ICorDebugAppDomain* ad, ICorDebugModule* module) override
    {
        std::lock_guard<std::recursive_mutex> hold(lock);
        ModuleInfo m;
        m.module = module;
        wchar_t name[1024] = L"";
        ULONG32 len = 0;
        module->GetName(1024, &len, name);
        m.path = name;
        ComPtr<IUnknown> unknown;
        if (SUCCEEDED(module->GetMetaDataInterface(IID_IMetaDataImport, &unknown))) unknown->QueryInterface(IID_IMetaDataImport, reinterpret_cast<void**>(m.metadata.GetAddressOf()));
        BOOL dynamic = FALSE, in_memory = FALSE;
        module->IsDynamic(&dynamic);
        module->IsInMemory(&in_memory);
        if (binder && m.metadata && !dynamic && !in_memory) binder->GetReaderForFile(m.metadata.Get(), name, nullptr, &m.symbols);
        ComPtr<ICorDebugModule2> module2;
        if (SUCCEEDED(module->QueryInterface(IID_ICorDebugModule2, &module2))) module2->SetJMCStatus(m.symbols ? TRUE : FALSE, 0, nullptr);
        CORDB_ADDRESS base = 0;
        module->GetBaseAddress(&base);
        ModuleInfo& stored = modules[base] = m;
        event("module", "{\"path\":" + json_quote(u8(name)) + ",\"symbols\":" + (m.symbols ? "true" : "false") + "}");
        for (auto& bp : breakpoints) if (!bp.verified) bind(bp, stored);
        return ad->Continue(FALSE);
    }
    HRESULT STDMETHODCALLTYPE UnloadModule(ICorDebugAppDomain* ad, ICorDebugModule*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE LoadClass(ICorDebugAppDomain* ad, ICorDebugClass*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE UnloadClass(ICorDebugAppDomain* ad, ICorDebugClass*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE DebuggerError(ICorDebugProcess*, HRESULT hr, DWORD) override
    {
        char buf[16];
        std::snprintf(buf, sizeof(buf), "0x%08lX", static_cast<unsigned long>(hr));
        event("error", std::string("{\"hresult\":\"") + buf + "\"}");
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE LogMessage(ICorDebugAppDomain* ad, ICorDebugThread*, LONG, WCHAR*, WCHAR* message) override
    {
        event("output", "{\"category\":\"log\",\"text\":" + json_quote(u8(message)) + "}");
        return ad->Continue(FALSE);
    }
    HRESULT STDMETHODCALLTYPE LogSwitch(ICorDebugAppDomain* ad, ICorDebugThread*, LONG, ULONG, WCHAR*, WCHAR*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE CreateAppDomain(ICorDebugProcess* p, ICorDebugAppDomain* ad) override { ad->Attach(); return p->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE ExitAppDomain(ICorDebugProcess* p, ICorDebugAppDomain*) override { return p->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE LoadAssembly(ICorDebugAppDomain* ad, ICorDebugAssembly*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE UnloadAssembly(ICorDebugAppDomain* ad, ICorDebugAssembly*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE ControlCTrap(ICorDebugProcess* p) override { return p->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE NameChange(ICorDebugAppDomain* ad, ICorDebugThread*) override { return ad ? ad->Continue(FALSE) : S_OK; }
    HRESULT STDMETHODCALLTYPE UpdateModuleSymbols(ICorDebugAppDomain* ad, ICorDebugModule*, IStream*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE EditAndContinueRemap(ICorDebugAppDomain* ad, ICorDebugThread*, ICorDebugFunction*, BOOL) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE BreakpointSetError(ICorDebugAppDomain* ad, ICorDebugThread*, ICorDebugBreakpoint*, DWORD) override { return ad->Continue(FALSE); }
    // ---- ICorDebugManagedCallback2
    HRESULT STDMETHODCALLTYPE FunctionRemapOpportunity(ICorDebugAppDomain* ad, ICorDebugThread*, ICorDebugFunction*, ICorDebugFunction*, ULONG32) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE CreateConnection(ICorDebugProcess* p, CONNID, WCHAR*) override { return p->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE ChangeConnection(ICorDebugProcess* p, CONNID) override { return p->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE DestroyConnection(ICorDebugProcess* p, CONNID) override { return p->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE Exception(ICorDebugAppDomain* ad, ICorDebugThread*, ICorDebugFrame*, ULONG32, CorDebugExceptionCallbackType, DWORD) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE ExceptionUnwind(ICorDebugAppDomain* ad, ICorDebugThread*, CorDebugExceptionUnwindCallbackType, DWORD) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE FunctionRemapComplete(ICorDebugAppDomain* ad, ICorDebugThread*, ICorDebugFunction*) override { return ad->Continue(FALSE); }
    HRESULT STDMETHODCALLTYPE MDANotification(ICorDebugController* c, ICorDebugThread*, ICorDebugMDA*) override { return c->Continue(FALSE); }

    // ---- launch
    std::string launch(const JsonValue& params)
    {
        if (process) fail("invalid_state", "本适配器已经在调试一个进程。");
        auto text = [&](const char* key) { const JsonValue* v = params.find(key); return v && v->type == JsonValue::Type::string ? v->text : std::string(); };
        std::wstring program = widen(text("program")), directory = widen(text("cwd")), arguments = widen(text("arguments"));
        if (program.empty() || GetFileAttributesW(program.c_str()) == INVALID_FILE_ATTRIBUTES) fail("invalid_argument", "找不到要调试的程序：" + text("program"));
        if (const JsonValue* list = params.find("breakpoints"))
            for (const auto& b : list->items) {
                SourceBreakpoint bp;
                bp.id = next_breakpoint++;
                bp.file = widen(b.find("file") ? b.find("file")->text : std::string());
                bp.line = b.find("line") ? static_cast<ULONG32>(b.find("line")->int_value) : 0;
                breakpoints.push_back(bp);
            }
        // CLR v2 debugging interface, independent of which runtime this adapter process itself uses.
        HMODULE mscoree = LoadLibraryW(L"mscoree.dll");
        using Create = HRESULT(STDAPICALLTYPE*)(int, LPCWSTR, IUnknown**);
        auto create = mscoree ? reinterpret_cast<Create>(GetProcAddress(mscoree, "CreateDebuggingInterfaceFromVersion")) : nullptr;
        if (!create) fail("dependency_missing", "mscoree.dll 不提供 CreateDebuggingInterfaceFromVersion。");
        ComPtr<IUnknown> unknown;
        check(create(CorDebugVersion_2_0, L"v2.0.50727", &unknown), "加载 CLR v2.0.50727 调试接口");
        check(unknown.As(&cordebug), "ICorDebug");
        check(cordebug->Initialize(), "ICorDebug::Initialize");
        check(cordebug->SetManagedHandler(static_cast<ICorDebugManagedCallback*>(this)), "SetManagedHandler");
        CoCreateInstance(CLSID_CorSymBinder_SxS, nullptr, CLSCTX_INPROC_SERVER, IID_ISymUnmanagedBinder, &binder);
        SECURITY_ATTRIBUTES inherit{ sizeof(inherit), nullptr, TRUE };
        HANDLE write = nullptr, input = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &inherit, OPEN_EXISTING, 0, nullptr);
        CreatePipe(&output_read, &write, &inherit, 0);
        SetHandleInformation(output_read, HANDLE_FLAG_INHERIT, 0);
        STARTUPINFOW si{ sizeof(si) };
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = input;
        si.hStdOutput = si.hStdError = write;
        PROCESS_INFORMATION pi{};
        std::wstring command = L"\"" + program + L"\"" + (arguments.empty() ? L"" : L" " + arguments);
        HRESULT hr;
        {
            std::lock_guard<std::recursive_mutex> hold(lock);
            hr = cordebug->CreateProcess(program.c_str(), command.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr,
                directory.empty() ? nullptr : directory.c_str(), &si, &pi, DEBUG_NO_SPECIAL_OPTIONS, &process);
        }
        CloseHandle(write);
        if (input != INVALID_HANDLE_VALUE) CloseHandle(input);
        check(hr, "启动被调试进程");
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        unsigned codepage = GetOEMCP();
        output_thread = std::thread([this, codepage] {
            std::string pending;
            char buffer[4096];
            DWORD got = 0;
            auto flush_line = [&](std::string line) {
                if (!line.empty() && line.back() == '\r') line.pop_back();
                int n = MultiByteToWideChar(codepage, 0, line.data(), static_cast<int>(line.size()), nullptr, 0);
                std::wstring w(static_cast<std::size_t>(n > 0 ? n : 0), L'\0');
                if (n > 0) MultiByteToWideChar(codepage, 0, line.data(), static_cast<int>(line.size()), w.data(), n);
                event("output", "{\"category\":\"stdout\",\"text\":" + json_quote(u8(w.c_str())) + "}");
            };
            while (ReadFile(output_read, buffer, sizeof(buffer), &got, nullptr) && got) {
                pending.append(buffer, got);
                for (std::size_t nl; (nl = pending.find('\n')) != std::string::npos;) { flush_line(pending.substr(0, nl)); pending.erase(0, nl + 1); }
            }
            if (!pending.empty()) flush_line(pending);
        });
        return "{\"pid\":" + std::to_string(pi.dwProcessId) + ",\"bitness\":" + std::to_string(sizeof(void*) * 8) + "}";
    }

    std::string add_breakpoint(const std::string& file, ULONG32 line)
    {
        SourceBreakpoint bp;
        bp.id = next_breakpoint++;
        bp.file = widen(file);
        bp.line = line;
        breakpoints.push_back(bp);
        SourceBreakpoint& stored = breakpoints.back();
        // Binding needs a synchronized process: allowed while stopped or before the program runs managed code.
        if (stopped) for (auto& [base, m] : modules) if (!stored.verified) bind(stored, m);
        return breakpoint_json(stored);
    }
    std::string remove_breakpoint(int id)
    {
        auto it = std::find_if(breakpoints.begin(), breakpoints.end(), [&](const SourceBreakpoint& b) { return b.id == id; });
        if (it == breakpoints.end()) fail("invalid_argument", "断点不存在。");
        for (auto& b : it->bound) b->Activate(FALSE);
        breakpoints.erase(it);
        return "{\"removed\":" + std::to_string(id) + "}";
    }
};

std::string handle(Debugger& d, const std::string& method, const JsonValue& params)
{
    auto integer = [&](const char* key, long long fallback) { const JsonValue* v = params.find(key); return v && v->integer ? v->int_value : fallback; };
    auto text = [&](const char* key) { const JsonValue* v = params.find(key); return v && v->type == JsonValue::Type::string ? v->text : std::string(); };
    if (method == "launch") return d.launch(params);
    if (method == "pause") {
        if (!d.process || d.exited) fail("invalid_state", "没有正在调试的进程。");
        if (d.stopped) fail("invalid_state", "已经处于暂停状态。");
        check(d.process->Stop(5000), "Stop");
        std::lock_guard<std::recursive_mutex> hold(d.lock);
        ComPtr<ICorDebugThreadEnum> threads;
        ComPtr<ICorDebugThread> first;
        ICorDebugThread* t = nullptr;
        ULONG got = 0;
        if (SUCCEEDED(d.process->EnumerateThreads(&threads)) && threads->Next(1, &t, &got) == S_OK && got) first.Attach(t);
        d.stop_at(nullptr, first.Get(), "pause", "");
        return "{}";
    }
    if (method == "terminate") {
        if (!d.process || d.exited) return "{\"terminated\":false}";
        d.process->Stop(5000);
        check(d.process->Terminate(1), "Terminate");
        return "{\"terminated\":true}";
    }
    if (method == "breakpoint.add") {
        // Binding needs a synchronized process: a running target is stopped briefly (without holding our lock,
        // so pending callbacks can finish), bound, then resumed.
        bool running = d.process && !d.exited && !d.stopped;
        if (running) check(d.process->Stop(5000), "Stop");
        std::lock_guard<std::recursive_mutex> hold(d.lock);
        bool was_stopped = d.stopped;
        if (running) d.stopped = true;
        std::string result = d.add_breakpoint(text("file"), static_cast<ULONG32>(integer("line", 0)));
        if (running) { d.stopped = was_stopped; d.process->Continue(FALSE); }
        return result;
    }
    std::lock_guard<std::recursive_mutex> hold(d.lock);
    if (method == "breakpoint.remove") return d.remove_breakpoint(static_cast<int>(integer("id", 0)));
    if (d.exited) fail("invalid_state", "调试目标已退出。");
    if (method == "continue") { if (!d.stopped) fail("invalid_state", "调试目标未处于暂停状态。"); d.resume(); return "{}"; }
    if (method == "step") { d.step(text("kind").empty() ? "over" : text("kind")); return "{}"; }
    if (method == "stack") { if (!d.stopped) fail("invalid_state", "调试目标未处于暂停状态。"); return "{\"frames\":" + d.stack_json(d.stopped_thread.Get()) + "}"; }
    if (method == "variables") {
        auto vars = d.variables(static_cast<std::size_t>(integer("frame", 0)));
        std::string j = "{\"variables\":[";
        for (std::size_t i = 0; i < vars.size(); ++i) {
            std::string type, value = d.format(vars[i].value.Get(), type);
            j += (i ? "," : "") + std::string("{\"name\":") + json_quote(vars[i].name) + ",\"kind\":\"" + vars[i].kind + "\",\"type\":" + json_quote(type) + ",\"value\":" + json_quote(value) + "}";
        }
        return j + "]}";
    }
    if (method == "evaluate") {
        std::string type, value = d.evaluate(static_cast<std::size_t>(integer("frame", 0)), text("expression"), type);
        return "{\"type\":" + json_quote(type) + ",\"value\":" + json_quote(value) + "}";
    }
    fail("method_not_supported", method);
}

} // namespace

int main()
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    static Debugger debugger;
    g_debugger = &debugger;
    std::printf("{\"type\":\"ready\",\"protocolVersion\":1,\"adapter\":\"clr2\",\"bitness\":%d}\n", static_cast<int>(sizeof(void*) * 8));
    std::fflush(stdout);
    char buffer[1 << 16];
    std::string line;
    while (std::fgets(buffer, sizeof(buffer), stdin)) {
        line += buffer;
        if (line.empty() || line.back() != '\n') continue;
        line.pop_back();
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::string id = "null";
        try {
            JsonValue request = parse_json(line);
            const JsonValue* idv = request.find("id");
            const JsonValue* method = request.find("method");
            if (idv && idv->type == JsonValue::Type::string) id = json_quote(idv->text);
            if (!method || method->type != JsonValue::Type::string) fail("invalid_request", "缺少 method。");
            static const JsonValue empty = parse_json("{}");
            const JsonValue* params = request.find("params");
            std::string result = handle(debugger, method->text, params && params->type == JsonValue::Type::object ? *params : empty);
            emit("{\"id\":" + id + ",\"ok\":true,\"result\":" + result + "}");
        } catch (const Failure& f) {
            emit("{\"id\":" + id + ",\"ok\":false,\"error\":{\"code\":" + json_quote(f.code) + ",\"message\":" + json_quote(f.message) + "}}");
        } catch (const std::exception& e) {
            emit("{\"id\":" + id + ",\"ok\":false,\"error\":{\"code\":\"adapter_error\",\"message\":" + json_quote(e.what()) + "}}");
        }
        line.clear();
    }
    // stdin closed: the kernel detached; never leave a debuggee suspended under a dead debugger.
    {
        std::lock_guard<std::recursive_mutex> hold(debugger.lock);
        if (debugger.process && !debugger.exited) { debugger.process->Stop(2000); debugger.process->Terminate(1); }
    }
    Sleep(500);
    if (debugger.cordebug) debugger.cordebug->Terminate();
    return 0;
}
