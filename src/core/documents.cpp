#include "documents.hpp"

#include <algorithm>
#include <atomic>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace openmse::core {

namespace {
constexpr std::uint64_t max_file_bytes = 64ull * 1024 * 1024;
constexpr std::size_t max_history = 4000;
constexpr std::size_t coalesce_limit = 64;

std::string win32_message(const char* action, DWORD code)
{
    wchar_t* buffer = nullptr;
    DWORD n = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, 0, reinterpret_cast<wchar_t*>(&buffer), 0, nullptr);
    std::string detail;
    if (n && buffer) {
        std::wstring w(buffer, n);
        while (!w.empty() && (w.back() == L'\r' || w.back() == L'\n' || w.back() == L' ' || w.back() == L'.')) w.pop_back();
        try { detail = narrow(w); } catch (...) {}
    }
    if (buffer) LocalFree(buffer);
    return std::string(action) + "：" + (detail.empty() ? "Windows 错误" : detail) + " (" + std::to_string(code) + ")";
}

std::wstring full_path(const std::string& utf8)
{
    if (utf8.empty()) throw KernelError(ErrorCode::invalid_argument, "路径不能为空。");
    std::wstring input;
    try { input = widen(utf8); } catch (const std::invalid_argument& e) { throw KernelError(ErrorCode::invalid_argument, e.what()); }
    DWORD n = GetFullPathNameW(input.c_str(), 0, nullptr, nullptr);
    if (n == 0) throw KernelError(ErrorCode::invalid_argument, win32_message("无法解析路径", GetLastError()));
    std::wstring out(n, L'\0');
    n = GetFullPathNameW(input.c_str(), n, out.data(), nullptr);
    out.resize(n);
    return out;
}

bool same_path(const std::wstring& a, const std::wstring& b)
{
    return !a.empty() && CompareStringOrdinal(a.c_str(), static_cast<int>(a.size()), b.c_str(), static_cast<int>(b.size()), TRUE) == CSTR_EQUAL;
}

std::string file_name(const std::wstring& path)
{
    std::size_t slash = path.find_last_of(L"\\/");
    return narrow(slash == std::wstring::npos ? path : path.substr(slash + 1));
}

struct Handle {
    HANDLE h = INVALID_HANDLE_VALUE;
    ~Handle() { if (h != INVALID_HANDLE_VALUE) CloseHandle(h); }
};

struct FileRead {
    bool exists = false;
    std::vector<unsigned char> bytes;
    FileIdentity identity;
    bool read_only = false;
};

// Reads a whole file; exists=false when it is absent. Other failures throw io_error.
FileRead read_file(const std::wstring& path)
{
    FileRead r;
    Handle f;
    f.h = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (f.h == INVALID_HANDLE_VALUE) {
        DWORD e = GetLastError();
        if (e == ERROR_FILE_NOT_FOUND || e == ERROR_PATH_NOT_FOUND) return r;
        throw KernelError(ErrorCode::io_error, win32_message("无法读取文件", e));
    }
    BY_HANDLE_FILE_INFORMATION info{};
    if (!GetFileInformationByHandle(f.h, &info)) throw KernelError(ErrorCode::io_error, win32_message("无法读取文件信息", GetLastError()));
    if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) throw KernelError(ErrorCode::invalid_argument, "目标是文件夹，不是文件。");
    std::uint64_t size = (static_cast<std::uint64_t>(info.nFileSizeHigh) << 32) | info.nFileSizeLow;
    if (size > max_file_bytes) throw KernelError(ErrorCode::unsupported, "文件超过内核文本缓冲区上限（64 MB）。");
    r.exists = true;
    r.identity = { true, info.dwVolumeSerialNumber, (static_cast<std::uint64_t>(info.nFileIndexHigh) << 32) | info.nFileIndexLow };
    r.read_only = (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
    r.bytes.resize(static_cast<std::size_t>(size));
    std::size_t done = 0;
    while (done < r.bytes.size()) {
        DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(r.bytes.size() - done, 1u << 24)), got = 0;
        if (!ReadFile(f.h, r.bytes.data() + done, chunk, &got, nullptr)) throw KernelError(ErrorCode::io_error, win32_message("无法读取文件", GetLastError()));
        if (got == 0) { r.bytes.resize(done); break; } // file shrank while reading
        done += got;
    }
    return r;
}

FileIdentity identity_of(const std::wstring& path)
{
    Handle f;
    f.h = CreateFileW(path.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
    if (f.h == INVALID_HANDLE_VALUE) return {};
    BY_HANDLE_FILE_INFORMATION info{};
    if (!GetFileInformationByHandle(f.h, &info)) return {};
    return { true, info.dwVolumeSerialNumber, (static_cast<std::uint64_t>(info.nFileIndexHigh) << 32) | info.nFileIndexLow };
}

void rebuild_lines(Document& d)
{
    d.line_starts.assign(1, 0);
    for (std::size_t i = 0; i < d.text.size(); ++i)
        if (d.text[i] == '\n') d.line_starts.push_back(i + 1);
}

std::string to_policy(const std::string& lf_text, NewlinePolicy policy)
{
    if (policy == NewlinePolicy::lf) return lf_text;
    std::string out;
    out.reserve(lf_text.size() + lf_text.size() / 16);
    for (char c : lf_text) { if (c == '\n') out.push_back('\r'); out.push_back(c); }
    return out;
}

std::string checked_text(const std::string& utf8)
{
    if (!valid_utf8(utf8)) throw KernelError(ErrorCode::invalid_argument, "文本不是有效的 UTF-8。");
    return normalize_newlines(utf8);
}

std::atomic<std::uint64_t> temp_counter{0};
} // namespace

std::size_t offset_of(const Document& d, Position p)
{
    if (p.line >= d.line_starts.size()) throw KernelError(ErrorCode::invalid_argument, "行号超出文档范围。");
    std::size_t i = d.line_starts[p.line];
    std::size_t end = p.line + 1 < d.line_starts.size() ? d.line_starts[p.line + 1] - 1 : d.text.size();
    std::uint32_t units = 0;
    while (units < p.utf16_column) {
        if (i >= end) throw KernelError(ErrorCode::invalid_argument, "列号超出行尾。");
        unsigned char c = static_cast<unsigned char>(d.text[i]);
        std::size_t n = c < 0x80 ? 1 : c < 0xE0 ? 2 : c < 0xF0 ? 3 : 4;
        std::uint32_t u = n == 4 ? 2 : 1;
        if (units + u > p.utf16_column) throw KernelError(ErrorCode::invalid_argument, "列号位于 UTF-16 代理对中间。");
        units += u; i += n;
    }
    return i;
}

Position position_of(const Document& d, std::size_t offset)
{
    auto it = std::upper_bound(d.line_starts.begin(), d.line_starts.end(), offset);
    std::size_t line = static_cast<std::size_t>(it - d.line_starts.begin()) - 1;
    std::size_t start = d.line_starts[line];
    return { static_cast<std::uint32_t>(line), static_cast<std::uint32_t>(utf16_length(std::string_view(d.text).substr(start, offset - start))) };
}

void DocumentService::check_revision(const Document& d, Revision expected)
{
    if (expected != d.revision)
        throw KernelError(ErrorCode::conflict, "文档版本已变化（期望 " + std::to_string(expected) + "，当前 " + std::to_string(d.revision) + "），请刷新后重试。");
}

Document& DocumentService::get(std::uint64_t id)
{
    auto it = documents_.find(id);
    if (it == documents_.end()) throw KernelError(ErrorCode::invalid_argument, "文档不存在或已关闭。");
    return it->second;
}

Document* DocumentService::find_open(const std::wstring& path, const FileIdentity& identity, std::uint64_t except)
{
    for (auto& [id, d] : documents_) {
        if (id == except || d.path.empty()) continue;
        if ((identity.valid && d.identity == identity) || same_path(d.path, path)) return &d;
    }
    return nullptr;
}

Document& DocumentService::create(const std::string& name, const std::string& text)
{
    Document d;
    d.id = next_id_++;
    d.name = name.empty() ? "未命名" : name;
    if (!valid_utf8(d.name)) throw KernelError(ErrorCode::invalid_argument, "文档名称不是有效的 UTF-8。");
    d.text = checked_text(text);
    rebuild_lines(d);
    d.state = next_state_++;
    // An empty untitled document is clean; seeded template text still has to be saved somewhere.
    d.persisted_state = d.text.empty() ? d.state : 0;
    auto [it, inserted] = documents_.emplace(d.id, std::move(d));
    (void)inserted;
    return it->second;
}

std::pair<Document*, bool> DocumentService::open(const std::string& utf8_path)
{
    std::wstring path = full_path(utf8_path);
    FileRead file = read_file(path);
    if (!file.exists) throw KernelError(ErrorCode::io_error, "文件不存在：" + narrow(path));
    if (Document* existing = find_open(path, file.identity, 0)) return { existing, true };
    DecodedText decoded;
    try { decoded = decode_disk_bytes(file.bytes); }
    catch (const std::runtime_error& e) { throw KernelError(ErrorCode::unsupported, e.what()); }
    bool crlf = false, lone = false;
    const std::string& raw = decoded.utf8;
    for (std::size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r') { if (i + 1 < raw.size() && raw[i + 1] == '\n') { crlf = true; ++i; } else lone = true; }
        else if (raw[i] == '\n') lone = true;
    }
    Document d;
    d.id = next_id_++;
    d.path = path;
    d.name = file_name(path);
    d.text = normalize_newlines(raw);
    rebuild_lines(d);
    d.encoding = decoded.encoding;
    d.newline = crlf ? NewlinePolicy::crlf : NewlinePolicy::lf;
    d.mixed_newlines = crlf && lone;
    d.read_only = file.read_only;
    d.state = d.persisted_state = next_state_++;
    d.disk_known = true;
    d.disk_size = file.bytes.size();
    d.disk_hash = fnv1a(file.bytes.data(), file.bytes.size());
    d.identity = file.identity;
    auto [it, inserted] = documents_.emplace(d.id, std::move(d));
    (void)inserted;
    return { &it->second, false };
}

void DocumentService::apply_raw(Document& d, std::size_t offset, std::size_t remove, const std::string& insert)
{
    d.text.replace(offset, remove, insert);
    rebuild_lines(d);
    ++d.revision;
}

void DocumentService::replace(Document& d, Revision expected, Position begin, Position end, const std::string& utf8_text, bool coalesce)
{
    check_revision(d, expected);
    std::string insert = checked_text(utf8_text);
    std::size_t b = offset_of(d, begin), e = offset_of(d, end);
    if (e < b) throw KernelError(ErrorCode::invalid_argument, "文本范围的结束位置早于起始位置。");
    std::string removed = d.text.substr(b, e - b);
    if (removed.empty() && insert.empty()) return;

    HistoryEntry* last = d.undo.empty() ? nullptr : &d.undo.back();
    bool merged = false;
    if (coalesce && last && !last->sealed && last->state_after == d.state
        && insert.size() <= coalesce_limit && removed.size() <= coalesce_limit
        && insert.find('\n') == std::string::npos && removed.find('\n') == std::string::npos) {
        if (removed.empty() && last->removed.empty() && b == last->offset + last->inserted.size()) {
            last->inserted += insert; merged = true;                         // continued typing
        } else if (insert.empty() && last->inserted.empty() && e == last->offset) {
            last->removed = removed + last->removed; last->offset = b; merged = true; // backspace run
        } else if (insert.empty() && last->inserted.empty() && b == last->offset) {
            last->removed += removed; merged = true;                         // delete-key run
        }
    }
    apply_raw(d, b, e - b, insert);
    std::uint64_t before = d.state;
    d.state = next_state_++;
    if (merged) last->state_after = d.state;
    else {
        d.undo.push_back({ b, std::move(removed), std::move(insert), before, d.state, false });
        if (d.undo.size() > max_history) d.undo.erase(d.undo.begin());
    }
    d.redo.clear();
}

std::optional<AppliedChange> DocumentService::replace_text(Document& d, Revision expected, const std::string& utf8_text)
{
    check_revision(d, expected);
    std::string next = checked_text(utf8_text);
    const std::string& old = d.text;
    std::size_t limit = std::min(old.size(), next.size()), prefix = 0, suffix = 0;
    while (prefix < limit && old[prefix] == next[prefix]) ++prefix;
    while (prefix > 0 && (static_cast<unsigned char>(old[prefix]) & 0xC0u) == 0x80u) --prefix; // code point boundary
    while (suffix < limit - prefix && old[old.size() - 1 - suffix] == next[next.size() - 1 - suffix]) ++suffix;
    while (suffix > 0 && (static_cast<unsigned char>(old[old.size() - suffix]) & 0xC0u) == 0x80u) --suffix;
    if (prefix == old.size() && prefix == next.size()) return std::nullopt;
    std::size_t old_end = old.size() - suffix;
    std::string inserted = next.substr(prefix, next.size() - suffix - prefix);
    AppliedChange change{ position_of(d, prefix), position_of(d, old_end), inserted };
    replace(d, expected, change.begin, change.end, inserted, false);
    return change;
}

AppliedChange DocumentService::undo(Document& d, Revision expected)
{
    check_revision(d, expected);
    if (d.undo.empty()) throw KernelError(ErrorCode::invalid_state, "没有可撤销的操作。");
    HistoryEntry entry = std::move(d.undo.back());
    d.undo.pop_back();
    AppliedChange change{ position_of(d, entry.offset), position_of(d, entry.offset + entry.inserted.size()), entry.removed };
    apply_raw(d, entry.offset, entry.inserted.size(), entry.removed);
    d.state = entry.state_before;
    entry.sealed = true;
    d.redo.push_back(std::move(entry));
    return change;
}

AppliedChange DocumentService::redo(Document& d, Revision expected)
{
    check_revision(d, expected);
    if (d.redo.empty()) throw KernelError(ErrorCode::invalid_state, "没有可重做的操作。");
    HistoryEntry entry = std::move(d.redo.back());
    d.redo.pop_back();
    AppliedChange change{ position_of(d, entry.offset), position_of(d, entry.offset + entry.removed.size()), entry.inserted };
    apply_raw(d, entry.offset, entry.removed.size(), entry.inserted);
    d.state = entry.state_after;
    entry.sealed = true;
    d.undo.push_back(std::move(entry));
    return change;
}

void DocumentService::save(Document& d, Revision expected, const std::string& utf8_path)
{
    check_revision(d, expected);
    std::wstring target = utf8_path.empty() ? d.path : full_path(utf8_path);
    if (target.empty()) throw KernelError(ErrorCode::invalid_state, "未命名文档需要先选择保存路径。");
    FileIdentity target_identity = identity_of(target);
    if (find_open(target, target_identity, d.id)) throw KernelError(ErrorCode::conflict, "目标文件已在另一个标签中打开。");
    bool same_file = !d.path.empty() && (same_path(target, d.path) || (target_identity.valid && target_identity == d.identity));
    if (same_file && d.disk_known) {
        FileRead current = read_file(target);
        if (!current.exists) throw KernelError(ErrorCode::conflict, "文件已被删除或移动，请使用另存为，避免丢失内容。");
        if (current.bytes.size() != d.disk_size || fnv1a(current.bytes.data(), current.bytes.size()) != d.disk_hash)
            throw KernelError(ErrorCode::conflict, "文件已被其他程序修改，请先重新打开或另存为，避免覆盖。");
    }
    std::vector<unsigned char> bytes;
    try { bytes = encode_disk_bytes(to_policy(d.text, d.newline), d.encoding); }
    catch (const std::runtime_error& e) { throw KernelError(ErrorCode::io_error, e.what()); }

    std::wstring temp = target + L".openmse-" + std::to_wstring(GetCurrentProcessId()) + L"-" + std::to_wstring(++temp_counter) + L".tmp";
    {
        Handle f;
        f.h = CreateFileW(temp.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (f.h == INVALID_HANDLE_VALUE) throw KernelError(ErrorCode::io_error, win32_message("无法创建临时文件", GetLastError()));
        std::size_t done = 0;
        bool ok = true;
        DWORD error = 0;
        while (ok && done < bytes.size()) {
            DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(bytes.size() - done, 1u << 24)), wrote = 0;
            ok = WriteFile(f.h, bytes.data() + done, chunk, &wrote, nullptr) != 0;
            if (!ok) error = GetLastError();
            done += wrote;
        }
        if (ok && !FlushFileBuffers(f.h)) { ok = false; error = GetLastError(); }
        if (!ok) { CloseHandle(f.h); f.h = INVALID_HANDLE_VALUE; DeleteFileW(temp.c_str()); throw KernelError(ErrorCode::io_error, win32_message("写入临时文件失败", error)); }
    }
    BOOL replaced = target_identity.valid
        ? ReplaceFileW(target.c_str(), temp.c_str(), nullptr, REPLACEFILE_IGNORE_MERGE_ERRORS, nullptr, nullptr)
        : MoveFileExW(temp.c_str(), target.c_str(), MOVEFILE_WRITE_THROUGH);
    if (!replaced) {
        DWORD error = GetLastError();
        DeleteFileW(temp.c_str());
        throw KernelError(ErrorCode::io_error, win32_message("保存文件失败", error));
    }
    d.path = target;
    d.name = file_name(target);
    d.disk_known = true;
    d.disk_size = bytes.size();
    d.disk_hash = fnv1a(bytes.data(), bytes.size());
    d.identity = identity_of(target);
    d.read_only = false;
    d.persisted_state = d.state;
    if (!d.undo.empty()) d.undo.back().sealed = true;
}

void DocumentService::close(Document& d, ClosePolicy policy)
{
    if (policy == ClosePolicy::refuse_dirty && d.dirty()) throw KernelError(ErrorCode::unsaved_document, "“" + d.name + "”有未保存的更改。");
    if (policy == ClosePolicy::save && d.dirty()) save(d, d.revision, std::string());
    documents_.erase(d.id);
}

} // namespace openmse::core
