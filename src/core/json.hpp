#pragma once
// Minimal JSON reader that keeps byte spans, so callers can patch source text in place
// (preserving formatting and unknown members) instead of re-serialising whole documents.
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace openmse::core {

struct JsonError : std::runtime_error { using std::runtime_error::runtime_error; };

struct JsonValue;
struct JsonMember;

struct JsonValue {
    enum class Type { null, boolean, number, string, array, object };
    Type type = Type::null;
    std::size_t begin = 0, end = 0;   // span of the value in the source
    std::size_t close = 0;            // offset of ']' or '}' for containers
    bool boolean = false;
    bool integer = false;             // number without fraction/exponent that fits in long long
    long long int_value = 0;
    std::string text;                 // decoded string value
    std::vector<JsonValue> items;     // array elements
    std::vector<JsonMember> members;  // object members, in source order

    // Case-insensitive (ASCII) lookup, matching JavaScriptSerializer's binding; first match wins.
    const JsonValue* find(std::string_view key) const;
    const JsonMember* find_member(std::string_view key) const;
};

struct JsonMember {
    std::string key;
    std::size_t key_begin = 0;
    JsonValue value;
};

JsonValue parse_json(std::string_view text); // throws JsonError

// JSON string literal. js_safe also escapes < > & ' like JavaScriptSerializer does.
std::string json_quote(std::string_view utf8, bool js_safe = false);

} // namespace openmse::core
