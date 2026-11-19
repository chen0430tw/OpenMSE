#pragma once
// Authoritative document buffers for OpenMSE.Core: revisions, undo history, encoding and safe saving.
#include "text.hpp"

#include <openmse/kernel/services.hpp>

#include <cstdint>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace openmse::core {

using kernel::ClosePolicy;
using kernel::ErrorCode;
using kernel::Position;
using kernel::Revision;

struct KernelError : std::runtime_error {
    ErrorCode code;
    KernelError(ErrorCode c, const std::string& m) : std::runtime_error(m), code(c) {}
};

enum class NewlinePolicy { lf, crlf };

struct FileIdentity {
    bool valid = false;
    unsigned long volume = 0;
    std::uint64_t index = 0;
    bool operator==(const FileIdentity& o) const { return valid && o.valid && volume == o.volume && index == o.index; }
};

struct HistoryEntry {
    std::size_t offset = 0;      // byte offset in the text before this entry was applied
    std::string removed, inserted;
    std::uint64_t state_before = 0, state_after = 0;
    bool sealed = false;         // never merge more typing into a sealed entry
};

struct Document {
    std::uint64_t id = 0;
    std::wstring path;           // empty while untitled
    std::string name;            // UTF-8 display name
    std::string text;            // UTF-8, LF only
    std::vector<std::size_t> line_starts{0};
    DiskEncoding encoding;
    NewlinePolicy newline = NewlinePolicy::crlf;
    bool mixed_newlines = false;
    bool read_only = false;
    Revision revision = 1;
    std::uint64_t state = 0, persisted_state = 0;
    bool disk_known = false;     // a disk fingerprint exists for `path`
    std::uint64_t disk_size = 0, disk_hash = 0;
    FileIdentity identity;
    std::vector<HistoryEntry> undo, redo;

    bool dirty() const { return state != persisted_state; }
};

// The view-facing description of one applied change, in coordinates of the text before it.
struct AppliedChange {
    Position begin, end;
    std::string text;
};

class DocumentService {
public:
    Document& create(const std::string& name, const std::string& text);
    // Returns the document and whether it was already open.
    std::pair<Document*, bool> open(const std::string& utf8_path);
    Document& get(std::uint64_t id);
    void replace(Document&, Revision expected, Position begin, Position end, const std::string& utf8_text, bool coalesce);
    // Commits a complete new text as the minimal single edit (one revision, one undo step).
    // Returns nullopt when nothing changed.
    std::optional<AppliedChange> replace_text(Document&, Revision expected, const std::string& utf8_text);
    static void check_revision(const Document&, Revision expected);
    AppliedChange undo(Document&, Revision expected);
    AppliedChange redo(Document&, Revision expected);
    void save(Document&, Revision expected, const std::string& utf8_path);
    void close(Document&, ClosePolicy);
    const std::map<std::uint64_t, Document>& all() const { return documents_; }

private:
    std::map<std::uint64_t, Document> documents_;
    std::uint64_t next_id_ = 1, next_state_ = 1;

    Document* find_open(const std::wstring& path, const FileIdentity& identity, std::uint64_t except);
    void apply_raw(Document&, std::size_t offset, std::size_t remove, const std::string& insert);
};

std::size_t offset_of(const Document&, Position);          // throws invalid_argument KernelError
Position position_of(const Document&, std::size_t offset);

} // namespace openmse::core
