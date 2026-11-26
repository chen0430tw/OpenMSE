#pragma once
// DesignService and ToolboxService: design nodes over document text, and design operations that
// become source patches. Nothing here touches files or UI; the text is always the kernel buffer.
#include "documents.hpp"
#include "json.hpp"

#include <optional>
#include <string>
#include <vector>

namespace openmse::core {

enum class DesignKind { none, html, form };

struct DesignNode {
    std::string id;            // "n<index>"; valid only for the revision it was read at
    std::string kind, name, text, back_color = "#FFFFFF", fore_color = "#202835";
    long long x = 0, y = 0, width = 120, height = 30;
    bool enabled = true;
    bool text_editable = true; // false for HTML elements with nested markup or void tags
    std::string event, event_args, handler; // default event, its argument type (forms), bound handler ("" when none)
    std::vector<std::pair<std::string, std::string>> handlers; // forms: every bound event -> handler
    bool password = false;     // HTML <input type=password>, for previews
};

struct DesignSnapshot {
    DesignKind kind = DesignKind::none;
    std::string error;         // non-empty when the source cannot be shown as a design
    std::string title;         // form only
    std::string handler;       // form only: Load handler
    std::vector<std::pair<std::string, std::string>> handlers; // form only: every bound form event
    long long width = 0, height = 0, canvas_width = 0, canvas_height = 0;
    std::vector<DesignNode> nodes;
    bool truncated = false;
};

struct ToolboxEntry {
    std::string id, name, family, family_label, control_type, markup, backend;
    bool has_markup = false;
};

class ToolboxService {
public:
    void load(const std::string& utf8_path);
    const std::vector<ToolboxEntry>& entries() const { return entries_; }
    const ToolboxEntry& find(const std::string& id) const;
    bool loaded() const { return loaded_; }
private:
    std::vector<ToolboxEntry> entries_;
    bool loaded_ = false;
};

DesignKind design_kind(const Document&);
DesignSnapshot design_snapshot(const Document&);

struct DesignResult {
    std::string text;                 // complete new LF text; committed by the caller as one edit
    std::optional<std::string> node;  // node to select after the change, in the new revision
    std::optional<std::size_t> caret; // set_event on HTML: byte offset of the handler body in `text`
};
// op: {"op":"add"|"set"|"delete"|"set_form", ...}; throws KernelError.
DesignResult design_apply(const Document&, const JsonValue& op, const ToolboxService&);
std::string design_export(const Document&, const std::string& language); // "csharp" | "vb"
// Generated half of a partial form class (fields, InitializeComponent, event wiring).
std::string design_generate(const Document&, const std::string& language, const std::string& class_name);

const char* design_kind_name(DesignKind);

} // namespace openmse::core
