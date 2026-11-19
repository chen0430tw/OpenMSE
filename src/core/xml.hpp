#pragma once
// Minimal non-validating XML reader for MSBuild project files: elements, attributes, text, comments,
// CDATA, processing instructions and the predefined/numeric entities. No DTDs or external entities.
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace openmse::core {

struct XmlNode {
    std::string name;                                        // local name (prefix stripped)
    std::vector<std::pair<std::string, std::string>> attributes;
    std::vector<XmlNode> children;
    std::string text;                                        // concatenated direct text content
    std::string attribute(std::string_view key) const;       // case-insensitive, "" when absent
    std::vector<const XmlNode*> all(std::string_view child) const;
    const XmlNode* first(std::string_view child) const;
};

XmlNode parse_xml(std::string_view utf8); // throws std::runtime_error with a UTF-8 message

} // namespace openmse::core
