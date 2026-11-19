#pragma once
// UTF-8 text utilities and disk encoding conversion for OpenMSE.Core.
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace openmse::core {

bool valid_utf8(std::string_view text);
// CRLF and lone CR become LF.
std::string normalize_newlines(std::string_view text);
std::size_t utf16_length(std::string_view utf8);

std::wstring widen(std::string_view utf8);   // throws std::invalid_argument on invalid UTF-8
std::string narrow(std::wstring_view wide);  // throws std::invalid_argument on lone surrogates

enum class EncodingKind { utf8, utf8_bom, utf16le, utf16be, codepage };
struct DiskEncoding {
    EncodingKind kind = EncodingKind::utf8_bom;
    unsigned codepage = 0; // only for EncodingKind::codepage
    std::string name() const;
};

struct DecodedText {
    std::string utf8;       // not yet newline-normalised
    DiskEncoding encoding;
};
// Throws std::runtime_error with a user-facing UTF-8 message when the bytes are not editable text.
DecodedText decode_disk_bytes(const std::vector<unsigned char>& bytes);
// Throws std::runtime_error when a character cannot be represented exactly in the target encoding.
std::vector<unsigned char> encode_disk_bytes(std::string_view utf8, const DiskEncoding& encoding);

std::uint64_t fnv1a(const unsigned char* data, std::size_t size);

} // namespace openmse::core
