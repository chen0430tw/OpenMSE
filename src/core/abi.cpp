// C ABI boundary of OpenMSE.Core. Converts C++ results and exceptions into JSON buffers and status codes.
#define OPENMSE_CORE_BUILD
#include <openmse/kernel/abi.h>

#include "design.hpp"
#include "documents.hpp"
#include "debug.hpp"
#include "hosts.hpp"
#include "tasks.hpp"
#include "workspace.hpp"

#include <cstring>
#include <deque>
#include <exception>
#include <mutex>
#include <new>
#include <string>
#include <string_view>

using namespace openmse::core;

namespace {

class Json {
public:
    Json& raw(std::string_view s) { out_.append(s); return *this; }
    Json& str(std::string_view s)
    {
        static const char hex[] = "0123456789abcdef";
        out_.push_back('"');
        for (unsigned char c : s) {
            switch (c) {
            case '"': out_ += "\\\""; break;
            case '\\': out_ += "\\\\"; break;
            case '\n': out_ += "\\n"; break;
            case '\r': out_ += "\\r"; break;
            case '\t': out_ += "\\t"; break;
            default:
                if (c < 0x20) { out_ += "\\u00"; out_.push_back(hex[c >> 4]); out_.push_back(hex[c & 15]); }
                else out_.push_back(static_cast<char>(c));
            }
        }
        out_.push_back('"');
        return *this;
    }
    Json& key(std::string_view k) { if (!out_.empty() && out_.back() != '{' && out_.back() != '[') out_.push_back(','); return str(k).raw(":"); }
    Json& num(std::uint64_t v) { out_ += std::to_string(v); return *this; }
    Json& boolean(bool v) { out_ += v ? "true" : "false"; return *this; }
    Json& comma() { out_.push_back(','); return *this; }
    std::string take() { return std::move(out_); }
private:
    std::string out_;
};

const char* code_name(ErrorCode c)
{
    switch (c) {
    case ErrorCode::unsupported: return "unsupported";
    case ErrorCode::invalid_argument: return "invalid_argument";
    case ErrorCode::invalid_state: return "invalid_state";
    case ErrorCode::conflict: return "conflict";
    case ErrorCode::cancelled: return "cancelled";
    case ErrorCode::timeout: return "timeout";
    case ErrorCode::host_unavailable: return "host_unavailable";
    case ErrorCode::host_busy: return "host_busy";
    case ErrorCode::host_exited: return "host_exited";
    case ErrorCode::dependency_missing: return "dependency_missing";
    case ErrorCode::protocol_error: return "protocol_error";
    case ErrorCode::unsaved_document: return "unsaved_document";
    case ErrorCode::io_error: return "io_error";
    case ErrorCode::backend_error: return "backend_error";
    }
    return "backend_error";
}

std::string newline_name(const Document& d)
{
    if (d.mixed_newlines) return "mixed";
    return d.newline == NewlinePolicy::crlf ? "crlf" : "lf";
}

void write_state(Json& j, const Document& d)
{
    j.key("id").num(d.id).key("revision").num(d.revision).key("dirty").boolean(d.dirty())
        .key("can_undo").boolean(!d.undo.empty()).key("can_redo").boolean(!d.redo.empty());
}

std::string snapshot_json(const Document& d, bool already_open = false)
{
    Json j;
    j.raw("{");
    write_state(j, d);
    j.key("name").str(d.name).key("path");
    if (d.path.empty()) j.raw("null"); else j.str(narrow(d.path));
    j.key("text").str(d.text).key("encoding").str(d.encoding.name()).key("newline").str(newline_name(d))
        .key("read_only").boolean(d.read_only).key("line_count").num(d.line_starts.size())
        .key("already_open").boolean(already_open).raw("}");
    return j.take();
}

std::string state_json(const Document& d)
{
    Json j;
    j.raw("{");
    write_state(j, d);
    j.raw("}");
    return j.take();
}

void write_edit(Json& j, const AppliedChange& c)
{
    j.key("edit").raw("{").key("begin").raw("{").key("line").num(c.begin.line).key("column").num(c.begin.utf16_column).raw("}")
        .key("end").raw("{").key("line").num(c.end.line).key("column").num(c.end.utf16_column).raw("}")
        .key("text").str(c.text).raw("}");
}

std::string change_json(const Document& d, const AppliedChange& c)
{
    Json j;
    j.raw("{");
    write_state(j, d);
    write_edit(j, c);
    j.raw("}");
    return j.take();
}

void write_design(Json& j, const Document& d)
{
    DesignSnapshot s = design_snapshot(d);
    j.raw("{").key("document").num(d.id).key("revision").num(d.revision).key("kind").str(design_kind_name(s.kind)).key("error");
    if (s.error.empty()) j.raw("null"); else j.str(s.error);
    j.key("title").str(s.title).key("width").num(static_cast<std::uint64_t>(s.width)).key("height").num(static_cast<std::uint64_t>(s.height))
        .key("canvas_width").num(static_cast<std::uint64_t>(s.canvas_width)).key("canvas_height").num(static_cast<std::uint64_t>(s.canvas_height))
        .key("truncated").boolean(s.truncated).key("nodes").raw("[");
    bool first = true;
    for (const auto& n : s.nodes) {
        if (!first) j.comma();
        first = false;
        j.raw("{").key("id").str(n.id).key("kind").str(n.kind).key("name").str(n.name).key("text").str(n.text)
            .key("x").num(static_cast<std::uint64_t>(n.x)).key("y").num(static_cast<std::uint64_t>(n.y))
            .key("width").num(static_cast<std::uint64_t>(n.width)).key("height").num(static_cast<std::uint64_t>(n.height))
            .key("back_color").str(n.back_color).key("fore_color").str(n.fore_color).key("enabled").boolean(n.enabled)
            .key("text_editable").boolean(n.text_editable).key("password").boolean(n.password).raw("}");
    }
    j.raw("]}");
}

void write_toolbox(Json& j, const ToolboxService& t)
{
    j.raw("{\"entries\":[");
    bool first = true;
    for (const auto& e : t.entries()) {
        if (!first) j.comma();
        first = false;
        j.raw("{").key("id").str(e.id).key("name").str(e.name).key("family").str(e.family).key("family_label").str(e.family_label)
            .key("control_type").str(e.control_type).key("backend").str(e.backend).key("markup");
        if (e.has_markup) j.str(e.markup); else j.raw("null");
        j.raw("}");
    }
    j.raw("]}");
}

std::string workspace_json(const Workspace& w, const std::vector<Tool>& tools)
{
    auto strings = [](Json& j, const auto& list, auto convert) {
        j.raw("[");
        bool first = true;
        for (const auto& s : list) { if (!first) j.comma(); first = false; j.str(convert(s)); }
        j.raw("]");
    };
    auto same = [](const std::string& s) { return s; };
    auto path = [](const std::wstring& s) { return narrow(s); };
    Json j;
    j.raw("{").key("id").num(w.id).key("path").str(narrow(w.path)).key("root").str(narrow(w.root)).key("solution").boolean(w.solution)
        .key("startup_project").str(w.startup_project).key("configurations");
    strings(j, w.configurations, same);
    j.key("msbuild");
    if (const Tool* t = choose_msbuild(tools, w)) j.raw("{").key("id").str(t->id).key("version").str(t->version).key("path").str(narrow(t->path)).raw("}");
    else j.raw("null");
    j.key("projects").raw("[");
    bool first = true;
    for (const auto& p : w.projects) {
        if (!first) j.comma();
        first = false;
        j.raw("{").key("id").str(p.id).key("name").str(p.name).key("kind").str(p.kind).key("path").str(narrow(p.path))
            .key("output_type").str(p.output_type).key("assembly_name").str(p.assembly_name).key("framework").str(p.framework)
            .key("tools_version").str(p.tools_version).key("load_error").str(p.load_error).key("executable").boolean(p.executable()).key("files");
        strings(j, p.files, path);
        j.key("project_references");
        strings(j, p.project_references, same);
        j.key("unresolved_references");
        strings(j, p.unresolved_references, path);
        j.key("assembly_references");
        strings(j, p.assembly_references, same);
        j.key("configurations").raw("[");
        bool first_config = true;
        for (const auto& c : p.configurations) {
            if (!first_config) j.comma();
            first_config = false;
            j.raw("{").key("solution_configuration").str(c.solution_configuration).key("project_configuration").str(c.project_configuration)
                .key("build").boolean(c.build).key("platform_target").str(c.platform_target).key("artifact").str(narrow(c.artifact)).raw("}");
        }
        j.raw("]}");
    }
    j.raw("]}");
    return j.take();
}

struct CommandInfo { const char* id; const char* label; };
constexpr CommandInfo commands[] = {
    { "file.save", "保存" }, { "file.close", "关闭当前文档" },
    { "edit.undo", "撤销" }, { "edit.redo", "重做" },
};

} // namespace

struct omk_kernel {
    // Event queue first: worker threads (tasks, hosts) publish into it, so it must outlive them.
    std::mutex events_lock;
    std::deque<std::string> events;
    std::uint64_t sequence = 0;

    std::mutex lock; // service state touched by ABI calls
    DocumentService documents;
    ToolboxService toolbox;
    WorkspaceService workspaces;
    std::vector<Tool> tools;
    bool tools_discovered = false;
    // Declared last so they are destroyed first: they stop and join their worker threads.
    std::unique_ptr<HostManager> hosts;
    std::unique_ptr<TaskManager> tasks;
    std::unique_ptr<DebugManager> debug;

    omk_kernel()
        : hosts(std::make_unique<HostManager>([this](const char* topic, const std::string& payload) { publish(topic, payload); })),
          tasks(std::make_unique<TaskManager>([this](const char* topic, const std::string& payload) { publish(topic, payload); })),
          debug(std::make_unique<DebugManager>([this](const char* topic, const std::string& payload) { publish(topic, payload); })) {}

    // Thread-safe; never takes `lock`.
    void publish(const char* topic, const std::string& payload)
    {
        std::lock_guard<std::mutex> hold(events_lock);
        Json j;
        j.raw("{").key("sequence").num(++sequence).key("topic").str(topic).key("payload").raw(payload).raw("}");
        events.push_back(j.take());
        if (events.size() > 20000) events.pop_front(); // a client that never polls must not grow without bound
    }
    const std::vector<Tool>& discovered_tools()
    {
        if (!tools_discovered) { tools = discover_tools(); tools_discovered = true; }
        return tools;
    }
};

namespace {

omk_status finish(omk_buffer* out, omk_status status, std::string body)
{
    if (!out) return status;
    out->data = nullptr; out->size = 0;
    char* data = new (std::nothrow) char[body.size() + 1];
    if (!data) return OMK_E_BACKEND_ERROR;
    std::memcpy(data, body.data(), body.size());
    data[body.size()] = '\0';
    out->data = data; out->size = body.size();
    return status;
}

omk_status fail(omk_buffer* out, ErrorCode code, std::string_view message)
{
    Json j;
    j.raw("{\"ok\":false,\"error\":{").key("code").str(code_name(code)).key("message").str(message).raw("}}");
    return finish(out, static_cast<omk_status>(code) + 1, j.take());
}

// Runs `body` (under the kernel lock unless `locked` is false) and translates every exception at the ABI boundary.
template<class F> omk_status guarded(omk_kernel* k, omk_buffer* out, F&& body, bool locked = true)
{
    if (out) { out->data = nullptr; out->size = 0; }
    if (!k) return fail(out, ErrorCode::invalid_argument, "内核句柄为空。");
    try {
        std::unique_lock<std::mutex> hold(k->lock, std::defer_lock);
        if (locked) hold.lock();
        std::string result = body();
        return finish(out, OMK_OK, "{\"ok\":true,\"result\":" + result + "}");
    } catch (const KernelError& e) {
        return fail(out, e.code, e.what());
    } catch (const std::bad_alloc&) {
        return fail(out, ErrorCode::backend_error, "内核内存不足。");
    } catch (const std::exception& e) {
        return fail(out, ErrorCode::backend_error, e.what());
    } catch (...) {
        return fail(out, ErrorCode::backend_error, "内核发生未知错误。");
    }
}

std::string bytes(const char* p, size_t n)
{
    if (!p && n) throw KernelError(ErrorCode::invalid_argument, "字符串指针为空。");
    return n ? std::string(p, n) : std::string();
}

std::string command_state(omk_kernel* k, const std::string& id, std::uint64_t doc)
{
    bool known = false;
    for (const auto& c : commands) known |= id == c.id;
    if (!known) throw KernelError(ErrorCode::unsupported, "未知命令：" + id);
    bool enabled = false;
    std::string reason;
    auto& all = k->documents.all();
    auto it = doc ? all.find(doc) : all.end();
    if (it == all.end()) reason = "没有活动文档。";
    else if (id == "edit.undo") { enabled = !it->second.undo.empty(); if (!enabled) reason = "没有可撤销的操作。"; }
    else if (id == "edit.redo") { enabled = !it->second.redo.empty(); if (!enabled) reason = "没有可重做的操作。"; }
    else if (id == "file.save") { enabled = !it->second.path.empty(); if (!enabled) reason = "未命名文档需要先选择保存路径。"; }
    else enabled = true;
    Json j;
    j.raw("{").key("id").str(id).key("enabled").boolean(enabled).key("checked").boolean(false).key("unavailable_reason").str(reason).raw("}");
    return j.take();
}

} // namespace

extern "C" {

OMK_API uint32_t OMK_CALL omk_abi_version(void) { return OMK_ABI_VERSION; }

OMK_API omk_status OMK_CALL omk_create(uint32_t client_abi_version, omk_kernel** out_kernel)
{
    if (!out_kernel) return OMK_E_INVALID_ARGUMENT;
    *out_kernel = nullptr;
    if (client_abi_version != OMK_ABI_VERSION) return OMK_E_ABI_MISMATCH;
    *out_kernel = new (std::nothrow) omk_kernel();
    return *out_kernel ? OMK_OK : OMK_E_BACKEND_ERROR;
}

OMK_API void OMK_CALL omk_destroy(omk_kernel* kernel) { delete kernel; }

OMK_API void OMK_CALL omk_buffer_free(omk_buffer* buffer)
{
    if (!buffer) return;
    delete[] buffer->data;
    buffer->data = nullptr; buffer->size = 0;
}

OMK_API omk_status OMK_CALL omk_document_create(omk_kernel* k, const char* name, size_t name_size, const char* text, size_t text_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Document& d = k->documents.create(bytes(name, name_size), bytes(text, text_size));
        k->publish("document.opened", state_json(d));
        return snapshot_json(d);
    });
}

OMK_API omk_status OMK_CALL omk_document_open(omk_kernel* k, const char* path, size_t path_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        auto [d, existing] = k->documents.open(bytes(path, path_size));
        if (!existing) k->publish("document.opened", state_json(*d));
        return snapshot_json(*d, existing);
    });
}

OMK_API omk_status OMK_CALL omk_document_snapshot(omk_kernel* k, uint64_t document, omk_buffer* out)
{
    return guarded(k, out, [&] { return snapshot_json(k->documents.get(document)); });
}

OMK_API omk_status OMK_CALL omk_document_replace(omk_kernel* k, uint64_t document, uint64_t expected_revision,
    uint32_t begin_line, uint32_t begin_column, uint32_t end_line, uint32_t end_column,
    const char* text, size_t text_size, uint32_t flags, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Document& d = k->documents.get(document);
        Revision before = d.revision;
        k->documents.replace(d, expected_revision, { begin_line, begin_column }, { end_line, end_column }, bytes(text, text_size), (flags & OMK_EDIT_COALESCE) != 0);
        std::string state = state_json(d);
        if (d.revision != before) k->publish("document.changed", state);
        return state;
    });
}

OMK_API omk_status OMK_CALL omk_document_undo(omk_kernel* k, uint64_t document, uint64_t expected_revision, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Document& d = k->documents.get(document);
        std::string result = change_json(d, k->documents.undo(d, expected_revision));
        k->publish("document.changed", state_json(d));
        return result;
    });
}

OMK_API omk_status OMK_CALL omk_document_redo(omk_kernel* k, uint64_t document, uint64_t expected_revision, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Document& d = k->documents.get(document);
        std::string result = change_json(d, k->documents.redo(d, expected_revision));
        k->publish("document.changed", state_json(d));
        return result;
    });
}

OMK_API omk_status OMK_CALL omk_document_save(omk_kernel* k, uint64_t document, uint64_t expected_revision, const char* path, size_t path_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Document& d = k->documents.get(document);
        k->documents.save(d, expected_revision, bytes(path, path_size));
        Json j;
        j.raw("{");
        write_state(j, d);
        j.key("persisted_revision").num(d.revision).key("path").str(narrow(d.path)).key("name").str(d.name).key("encoding").str(d.encoding.name()).raw("}");
        std::string result = j.take();
        k->publish("document.saved", result);
        return result;
    });
}

OMK_API omk_status OMK_CALL omk_document_close(omk_kernel* k, uint64_t document, uint32_t policy, omk_buffer* out)
{
    return guarded(k, out, [&] {
        if (policy > OMK_CLOSE_DISCARD) throw KernelError(ErrorCode::invalid_argument, "未知的关闭策略。");
        Document& d = k->documents.get(document);
        std::uint64_t id = d.id;
        k->documents.close(d, static_cast<ClosePolicy>(policy));
        std::string result = "{\"id\":" + std::to_string(id) + "}";
        k->publish("document.closed", result);
        return result;
    });
}

OMK_API omk_status OMK_CALL omk_command_list(omk_kernel* k, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Json j;
        j.raw("{\"commands\":[");
        bool first = true;
        for (const auto& c : commands) {
            if (!first) j.comma();
            first = false;
            j.raw("{").key("id").str(c.id).key("label").str(c.label).key("required_capability").str("documents").raw("}");
        }
        j.raw("]}");
        return j.take();
    });
}

OMK_API omk_status OMK_CALL omk_command_query(omk_kernel* k, const char* id, size_t id_size, uint64_t document, omk_buffer* out)
{
    return guarded(k, out, [&] { return command_state(k, bytes(id, id_size), document); });
}

OMK_API omk_status OMK_CALL omk_command_execute(omk_kernel* k, const char* id, size_t id_size, uint64_t document, uint64_t expected_revision, omk_buffer* out)
{
    // Commands route to the same document operations as the direct calls, so menus and toolbars
    // cannot drift into writing files on their own.
    std::string name;
    try { name = bytes(id, id_size); } catch (...) { return fail(out, ErrorCode::invalid_argument, "命令 ID 无效。"); }
    if (name == "edit.undo") return omk_document_undo(k, document, expected_revision, out);
    if (name == "edit.redo") return omk_document_redo(k, document, expected_revision, out);
    if (name == "file.save") return omk_document_save(k, document, expected_revision, nullptr, 0, out);
    if (name == "file.close") return omk_document_close(k, document, OMK_CLOSE_REFUSE_DIRTY, out);
    return fail(out, ErrorCode::unsupported, "未知命令：" + name);
}

OMK_API omk_status OMK_CALL omk_toolbox_load(omk_kernel* k, const char* path, size_t path_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        k->toolbox.load(bytes(path, path_size));
        Json j;
        write_toolbox(j, k->toolbox);
        return j.take();
    });
}

OMK_API omk_status OMK_CALL omk_toolbox_list(omk_kernel* k, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Json j;
        write_toolbox(j, k->toolbox);
        return j.take();
    });
}

OMK_API omk_status OMK_CALL omk_design_snapshot(omk_kernel* k, uint64_t document, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Json j;
        write_design(j, k->documents.get(document));
        return j.take();
    });
}

OMK_API omk_status OMK_CALL omk_design_apply(omk_kernel* k, uint64_t document, uint64_t expected_revision, const char* op_json, size_t op_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Document& d = k->documents.get(document);
        // Node IDs belong to a revision: check it before interpreting the operation.
        DocumentService::check_revision(d, expected_revision);
        std::string text = bytes(op_json, op_size);
        if (!valid_utf8(text)) throw KernelError(ErrorCode::invalid_argument, "设计操作不是有效的 UTF-8。");
        JsonValue op;
        try { op = parse_json(text); } catch (const JsonError& e) { throw KernelError(ErrorCode::invalid_argument, std::string("设计操作 JSON 无效：") + e.what()); }
        DesignResult result = design_apply(d, op, k->toolbox);
        std::optional<AppliedChange> change = k->documents.replace_text(d, expected_revision, result.text);
        Json j;
        j.raw("{");
        write_state(j, d);
        if (change) write_edit(j, *change); else j.key("edit").raw("null");
        j.key("node");
        if (result.node) j.str(*result.node); else j.raw("null");
        j.key("design");
        write_design(j, d);
        j.raw("}");
        if (change) k->publish("document.changed", state_json(d));
        return j.take();
    });
}

OMK_API omk_status OMK_CALL omk_design_export(omk_kernel* k, uint64_t document, const char* language, size_t language_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Json j;
        j.raw("{").key("source").str(design_export(k->documents.get(document), bytes(language, language_size))).raw("}");
        return j.take();
    });
}

OMK_API omk_status OMK_CALL omk_tools_list(omk_kernel* k, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Json j;
        j.raw("{\"tools\":[");
        bool first = true;
        for (const auto& t : k->discovered_tools()) {
            if (!first) j.comma();
            first = false;
            j.raw("{").key("id").str(t.id).key("kind").str(t.kind).key("framework").str(t.framework).key("version").str(t.version).key("path").str(narrow(t.path)).raw("}");
        }
        j.raw("]}");
        return j.take();
    });
}

OMK_API omk_status OMK_CALL omk_workspace_open(omk_kernel* k, const char* path, size_t path_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Workspace& w = k->workspaces.open(bytes(path, path_size));
        std::string result = workspace_json(w, k->discovered_tools());
        k->publish("workspace.opened", "{\"id\":" + std::to_string(w.id) + "}");
        return result;
    });
}

OMK_API omk_status OMK_CALL omk_workspace_snapshot(omk_kernel* k, uint64_t workspace, omk_buffer* out)
{
    return guarded(k, out, [&] { return workspace_json(k->workspaces.get(workspace), k->discovered_tools()); });
}

OMK_API omk_status OMK_CALL omk_workspace_close(omk_kernel* k, uint64_t workspace, omk_buffer* out)
{
    return guarded(k, out, [&] {
        k->workspaces.get(workspace);
        for (const auto& t : k->tasks->list())
            if (t.workspace == workspace && t.state == TaskState::running) throw KernelError(ErrorCode::invalid_state, "工作区仍有运行中的任务。");
        k->workspaces.close(workspace);
        k->publish("workspace.closed", "{\"id\":" + std::to_string(workspace) + "}");
        return "{\"id\":" + std::to_string(workspace) + "}";
    });
}

OMK_API omk_status OMK_CALL omk_build_start(omk_kernel* k, uint64_t workspace, const char* configuration, size_t configuration_size, const char* project, size_t project_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        const Workspace& w = k->workspaces.get(workspace);
        std::uint64_t id = k->tasks->start_build(w, k->discovered_tools(), bytes(configuration, configuration_size), bytes(project, project_size));
        return TaskManager::json(k->tasks->snapshot(id));
    });
}

OMK_API omk_status OMK_CALL omk_run_start(omk_kernel* k, uint64_t workspace, const char* configuration, size_t configuration_size, const char* project, size_t project_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        const Workspace& w = k->workspaces.get(workspace);
        std::uint64_t id = k->tasks->start_run(w, bytes(configuration, configuration_size), bytes(project, project_size));
        return TaskManager::json(k->tasks->snapshot(id));
    });
}

OMK_API omk_status OMK_CALL omk_task_cancel(omk_kernel* k, uint64_t task, omk_buffer* out)
{
    return guarded(k, out, [&] { k->tasks->cancel(task); return TaskManager::json(k->tasks->snapshot(task)); }, false);
}

OMK_API omk_status OMK_CALL omk_task_snapshot(omk_kernel* k, uint64_t task, omk_buffer* out)
{
    return guarded(k, out, [&] { return TaskManager::json(k->tasks->snapshot(task)); }, false);
}

OMK_API omk_status OMK_CALL omk_task_wait(omk_kernel* k, uint64_t task, uint32_t timeout_ms, omk_buffer* out)
{
    // Waits without the service lock, so other threads keep using the kernel.
    return guarded(k, out, [&] {
        bool finished = false;
        TaskInfo info = k->tasks->wait(task, timeout_ms, finished);
        return std::string("{\"finished\":") + (finished ? "true" : "false") + ",\"task\":" + TaskManager::json(info) + "}";
    }, false);
}

// ---- Legacy hosts (ABI 4). Host calls never hold the service lock while waiting.

OMK_API omk_status OMK_CALL omk_hosts_load(omk_kernel* k, const char* manifest, size_t manifest_size, const char* root, size_t root_size, omk_buffer* out)
{
    return guarded(k, out, [&] { return k->hosts->load(bytes(manifest, manifest_size), bytes(root, root_size)); }, false);
}

OMK_API omk_status OMK_CALL omk_host_start(omk_kernel* k, const char* engine, size_t engine_size, const char* workspace, size_t workspace_size, omk_buffer* out)
{
    return guarded(k, out, [&] { return k->hosts->snapshot(k->hosts->start(bytes(engine, engine_size), bytes(workspace, workspace_size))); }, false);
}

OMK_API omk_status OMK_CALL omk_host_list(omk_kernel* k, omk_buffer* out) { return guarded(k, out, [&] { return k->hosts->list(); }, false); }
OMK_API omk_status OMK_CALL omk_host_snapshot(omk_kernel* k, uint64_t host, omk_buffer* out) { return guarded(k, out, [&] { return k->hosts->snapshot(host); }, false); }

OMK_API omk_status OMK_CALL omk_host_wait(omk_kernel* k, uint64_t host, uint32_t timeout_ms, omk_buffer* out)
{
    return guarded(k, out, [&] {
        bool settled = false;
        std::string h = k->hosts->wait(host, timeout_ms, settled);
        return std::string("{\"settled\":") + (settled ? "true" : "false") + ",\"host\":" + h + "}";
    }, false);
}

OMK_API omk_status OMK_CALL omk_host_request(omk_kernel* k, uint64_t host, const char* method, size_t method_size, const char* params, size_t params_size, uint32_t timeout_ms, omk_buffer* out)
{
    return guarded(k, out, [&] {
        std::string p = bytes(params, params_size);
        if (!valid_utf8(p)) throw KernelError(ErrorCode::invalid_argument, "params 不是有效的 UTF-8。");
        return "{\"request\":" + std::to_string(k->hosts->request(host, bytes(method, method_size), p, timeout_ms)) + "}";
    }, false);
}

OMK_API omk_status OMK_CALL omk_host_sync_document(omk_kernel* k, uint64_t host, uint64_t document, uint32_t timeout_ms, omk_buffer* out)
{
    return guarded(k, out, [&] {
        Document& d = k->documents.get(document);
        if (d.path.empty()) throw KernelError(ErrorCode::invalid_state, "未命名文档需要先保存，才能同步到宿主。");
        return "{\"request\":" + std::to_string(k->hosts->sync_document(host, d.id, narrow(d.path), d.text, timeout_ms)) + ",\"revision\":" + std::to_string(d.revision) + "}";
    });
}

OMK_API omk_status OMK_CALL omk_host_request_wait(omk_kernel* k, uint64_t host, uint64_t request, uint32_t timeout_ms, omk_buffer* out)
{
    return guarded(k, out, [&] {
        bool finished = false;
        std::string r = k->hosts->wait_request(host, request, timeout_ms, finished);
        return std::string("{\"finished\":") + (finished ? "true" : "false") + ",\"request\":" + r + "}";
    }, false);
}

OMK_API omk_status OMK_CALL omk_host_verify(omk_kernel* k, uint64_t host, uint32_t timeout_ms, omk_buffer* out)
{
    return guarded(k, out, [&] { return "{\"request\":" + std::to_string(k->hosts->verify(host, timeout_ms)) + "}"; }, false);
}

OMK_API omk_status OMK_CALL omk_host_reconnect(omk_kernel* k, uint64_t host, omk_buffer* out)
{
    return guarded(k, out, [&] { k->hosts->reconnect(host); return k->hosts->snapshot(host); }, false);
}

OMK_API omk_status OMK_CALL omk_host_stop(omk_kernel* k, uint64_t host, const char* mode, size_t mode_size, omk_buffer* out)
{
    return guarded(k, out, [&] {
        std::uint64_t request = k->hosts->stop(host, bytes(mode, mode_size));
        return "{\"request\":" + (request ? std::to_string(request) : std::string("null")) + ",\"host\":" + k->hosts->snapshot(host) + "}";
    }, false);
}

// ---- Debugging (ABI 5)

OMK_API omk_status OMK_CALL omk_debug_start(omk_kernel* k, uint64_t workspace, const char* configuration, size_t configuration_size,
    const char* project, size_t project_size, const char* breakpoints, size_t breakpoints_size, omk_buffer* out)
{
    // The project graph is copied under the service lock; the adapter is then started without holding it.
    return guarded(k, out, [&] {
        Workspace w;
        {
            std::lock_guard<std::mutex> hold(k->lock);
            w = k->workspaces.get(workspace);
        }
        return k->debug->snapshot(k->debug->start(w, bytes(configuration, configuration_size), bytes(project, project_size), bytes(breakpoints, breakpoints_size)));
    }, false);
}

OMK_API omk_status OMK_CALL omk_debug_snapshot(omk_kernel* k, uint64_t session, omk_buffer* out) { return guarded(k, out, [&] { return k->debug->snapshot(session); }, false); }

OMK_API omk_status OMK_CALL omk_debug_wait(omk_kernel* k, uint64_t session, uint32_t timeout_ms, omk_buffer* out)
{
    return guarded(k, out, [&] {
        bool settled = false;
        std::string s = k->debug->wait(session, timeout_ms, settled);
        return std::string("{\"settled\":") + (settled ? "true" : "false") + ",\"session\":" + s + "}";
    }, false);
}

OMK_API omk_status OMK_CALL omk_debug_control(omk_kernel* k, uint64_t session, const char* action, size_t action_size, omk_buffer* out)
{
    return guarded(k, out, [&] { return k->debug->control(session, bytes(action, action_size)); }, false);
}

OMK_API omk_status OMK_CALL omk_debug_breakpoint_add(omk_kernel* k, uint64_t session, const char* file, size_t file_size, uint32_t line, omk_buffer* out)
{
    return guarded(k, out, [&] { return k->debug->add_breakpoint(session, bytes(file, file_size), line); }, false);
}

OMK_API omk_status OMK_CALL omk_debug_breakpoint_remove(omk_kernel* k, uint64_t session, uint32_t id, omk_buffer* out)
{
    return guarded(k, out, [&] { return k->debug->remove_breakpoint(session, id); }, false);
}

OMK_API omk_status OMK_CALL omk_debug_variables(omk_kernel* k, uint64_t session, uint32_t frame, omk_buffer* out)
{
    return guarded(k, out, [&] { return k->debug->variables(session, frame); }, false);
}

OMK_API omk_status OMK_CALL omk_debug_evaluate(omk_kernel* k, uint64_t session, uint32_t frame, const char* expression, size_t expression_size, omk_buffer* out)
{
    return guarded(k, out, [&] { return k->debug->evaluate(session, frame, bytes(expression, expression_size)); }, false);
}

// Capability registry: what each provider can do and how strongly that is evidenced on this machine.
// "behavior_verified" entries are covered by tests/kernel (core-tests, host-tests); blocked ones carry the reason.
OMK_API omk_status OMK_CALL omk_capabilities(omk_kernel* k, omk_buffer* out)
{
    return guarded(k, out, [&] {
        struct Cap { const char* name; const char* provider; const char* evidence; const char* note; };
        static const Cap caps[] = {
            { "documents", "core", "behavior_verified", "UTF-8/UTF-16/code-page files, revisions, undo, conflict-safe save" },
            { "design.html", "core", "behavior_verified", "source-span patches; static layout preview" },
            { "design.omform", "core", "behavior_verified", "in-place JSON patches; C#/VB export" },
            { "workspace.msbuild", "core", "behavior_verified", ".sln/.csproj/.vbproj project graph" },
            { "build.msbuild", "core", "behavior_verified", "asynchronous tasks, diagnostics, cancellation" },
            { "run", "core", "behavior_verified", "startup project with captured output" },
            { "language.diagnostics", "build.msbuild", "behavior_verified", "compiler diagnostics with file/line/column from builds" },
            { "language.completion", "none", "unsupported", "no completion provider" },
            { "language.definitions", "none", "unsupported", "no go-to-definition provider" },
            { "debug.clr2", "OpenMSE.DebugHost", "behavior_verified", "x86/x64 CLR 2.0: breakpoints, stepping, stack, locals, field evaluation; no function evaluation" },
            { "host.vs8.documents", "vs8", "behavior_verified", "documents.sync over bridge protocol v2" },
            { "host.vs8.lifecycle", "vs8", "behavior_verified", "owned start, timeout/verify, reconnect, restart, shutdown" },
            { "host.vs8.debug", "vs8", "blocked", "EnvDTE Debugger.Go returns 0x80004005 (hidden and visible instances)" },
            { "host.mse.automation", "mse", "blocked", "no DTE in the running object table; NativeOM probe returns 0x80004005" },
        };
        Json j;
        j.raw("{\"capabilities\":[");
        bool first = true;
        for (const auto& c : caps) {
            if (!first) j.comma();
            first = false;
            j.raw("{").key("name").str(c.name).key("provider").str(c.provider).key("evidence").str(c.evidence).key("note").str(c.note).raw("}");
        }
        j.raw("]}");
        return j.take();
    }, false);
}

OMK_API omk_status OMK_CALL omk_event_poll(omk_kernel* k, omk_buffer* out)
{
    return guarded(k, out, [&] {
        std::lock_guard<std::mutex> hold(k->events_lock);
        if (k->events.empty()) return std::string("{\"event\":null}");
        std::string e = "{\"event\":" + k->events.front() + "}";
        k->events.pop_front();
        return e;
    }, false);
}

} // extern "C"
