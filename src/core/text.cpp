#include "text.hpp"

#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace openmse::core {

namespace {
// Returns the byte length of the UTF-8 sequence at text[i], or 0 if it is invalid.
std::size_t sequence_length(std::string_view text, std::size_t i)
{
    auto at = [&](std::size_t k) { return static_cast<unsigned char>(text[k]); };
    unsigned char lead = at(i);
    if (lead < 0x80) return 1;
    std::size_t n; std::uint32_t cp;
    if (lead >= 0xC2 && lead <= 0xDF) { n = 2; cp = lead & 0x1Fu; }
    else if (lead >= 0xE0 && lead <= 0xEF) { n = 3; cp = lead & 0x0Fu; }
    else if (lead >= 0xF0 && lead <= 0xF4) { n = 4; cp = lead & 0x07u; }
    else return 0;
    if (i + n > text.size()) return 0;
    for (std::size_t k = 1; k < n; ++k) {
        unsigned char c = at(i + k);
        if ((c & 0xC0u) != 0x80u) return 0;
        cp = (cp << 6) | (c & 0x3Fu);
    }
    if ((n == 3 && cp < 0x800) || (n == 4 && (cp < 0x10000 || cp > 0x10FFFF))) return 0; // overlong / range
    if (cp >= 0xD800 && cp <= 0xDFFF) return 0; // surrogates are not scalar values
    return n;
}

std::string message(const char* text) { return std::string(text); }
} // namespace

bool valid_utf8(std::string_view text)
{
    for (std::size_t i = 0; i < text.size();) {
        std::size_t n = sequence_length(text, i);
        if (n == 0) return false;
        i += n;
    }
    return true;
}

std::string normalize_newlines(std::string_view text)
{
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (c == '\r') { out.push_back('\n'); if (i + 1 < text.size() && text[i + 1] == '\n') ++i; }
        else out.push_back(c);
    }
    return out;
}

std::size_t utf16_length(std::string_view utf8)
{
    std::size_t units = 0;
    for (unsigned char c : utf8) {
        if ((c & 0xC0u) == 0x80u) continue;
        units += c >= 0xF0 ? 2 : 1;
    }
    return units;
}

std::wstring widen(std::string_view utf8)
{
    if (utf8.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
    if (n <= 0) throw std::invalid_argument("文本不是有效的 UTF-8。");
    std::wstring out(static_cast<std::size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8.data(), static_cast<int>(utf8.size()), out.data(), n);
    return out;
}

std::string narrow(std::wstring_view wide)
{
    if (wide.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
    if (n <= 0) throw std::invalid_argument("文本包含无法配对的 UTF-16 代理项。");
    std::string out(static_cast<std::size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide.data(), static_cast<int>(wide.size()), out.data(), n, nullptr, nullptr);
    return out;
}

std::string DiskEncoding::name() const
{
    switch (kind) {
    case EncodingKind::utf8: return "utf-8";
    case EncodingKind::utf8_bom: return "utf-8-bom";
    case EncodingKind::utf16le: return "utf-16le";
    case EncodingKind::utf16be: return "utf-16be";
    case EncodingKind::codepage: return "cp" + std::to_string(codepage);
    }
    return "unknown";
}

namespace {
std::string decode_utf16(const unsigned char* data, std::size_t size, bool big_endian)
{
    if (size % 2 != 0) throw std::runtime_error(message("UTF-16 文件长度不完整，请使用原版兼容工具打开。"));
    std::wstring wide(size / 2, L'\0');
    for (std::size_t i = 0; i < wide.size(); ++i) {
        unsigned a = data[2 * i], b = data[2 * i + 1];
        wide[i] = static_cast<wchar_t>(big_endian ? (a << 8) | b : (b << 8) | a);
    }
    try { return narrow(wide); }
    catch (const std::invalid_argument&) { throw std::runtime_error(message("UTF-16 文件包含无效代理项，请使用原版兼容工具打开。")); }
}

std::string decode_codepage(const unsigned char* data, std::size_t size, unsigned codepage)
{
    if (size == 0) return {};
    const char* bytes = reinterpret_cast<const char*>(data);
    int n = MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS, bytes, static_cast<int>(size), nullptr, 0);
    if (n <= 0) throw std::runtime_error(message("无法识别文件编码，请使用原版兼容工具打开。"));
    std::wstring wide(static_cast<std::size_t>(n), L'\0');
    MultiByteToWideChar(codepage, MB_ERR_INVALID_CHARS, bytes, static_cast<int>(size), wide.data(), n);
    return narrow(wide);
}
} // namespace

DecodedText decode_disk_bytes(const std::vector<unsigned char>& bytes)
{
    const unsigned char* data = bytes.data();
    std::size_t size = bytes.size();
    if (size >= 2 && data[0] == 0xFF && data[1] == 0xFE)
        return { decode_utf16(data + 2, size - 2, false), { EncodingKind::utf16le, 0 } };
    if (size >= 2 && data[0] == 0xFE && data[1] == 0xFF)
        return { decode_utf16(data + 2, size - 2, true), { EncodingKind::utf16be, 0 } };
    for (std::size_t i = 0; i < size; ++i)
        if (data[i] == 0) throw std::runtime_error(message("该文件似乎是二进制文件，请使用原版兼容工具打开。"));
    if (size >= 3 && data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
        std::string_view body(reinterpret_cast<const char*>(data + 3), size - 3);
        if (!valid_utf8(body)) throw std::runtime_error(message("文件标记为 UTF-8，但内容不是有效的 UTF-8。"));
        return { std::string(body), { EncodingKind::utf8_bom, 0 } };
    }
    std::string_view body(reinterpret_cast<const char*>(data), size);
    if (valid_utf8(body)) return { std::string(body), { EncodingKind::utf8, 0 } };
    unsigned acp = GetACP();
    return { decode_codepage(data, size, acp), { EncodingKind::codepage, acp } };
}

std::vector<unsigned char> encode_disk_bytes(std::string_view utf8, const DiskEncoding& encoding)
{
    std::vector<unsigned char> out;
    switch (encoding.kind) {
    case EncodingKind::utf8_bom:
        out = { 0xEF, 0xBB, 0xBF };
        [[fallthrough]];
    case EncodingKind::utf8:
        out.insert(out.end(), utf8.begin(), utf8.end());
        return out;
    case EncodingKind::utf16le:
    case EncodingKind::utf16be: {
        bool be = encoding.kind == EncodingKind::utf16be;
        std::wstring wide = widen(utf8);
        out.reserve(2 + wide.size() * 2);
        out.push_back(be ? 0xFE : 0xFF); out.push_back(be ? 0xFF : 0xFE);
        for (wchar_t w : wide) {
            unsigned char hi = static_cast<unsigned char>(w >> 8), lo = static_cast<unsigned char>(w & 0xFF);
            if (be) { out.push_back(hi); out.push_back(lo); } else { out.push_back(lo); out.push_back(hi); }
        }
        return out;
    }
    case EncodingKind::codepage: {
        std::wstring wide = widen(utf8);
        if (wide.empty()) return out;
        BOOL used_default = FALSE;
        int n = WideCharToMultiByte(encoding.codepage, WC_NO_BEST_FIT_CHARS, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, &used_default);
        if (n <= 0 || used_default) throw std::runtime_error("文本包含 " + encoding.name() + " 编码无法表示的字符，请另存为 UTF-8。");
        std::string bytes(static_cast<std::size_t>(n), '\0');
        WideCharToMultiByte(encoding.codepage, WC_NO_BEST_FIT_CHARS, wide.data(), static_cast<int>(wide.size()), bytes.data(), n, nullptr, &used_default);
        // Fail rather than silently substituting: the bytes must decode back to exactly the same text.
        if (used_default || decode_codepage(reinterpret_cast<const unsigned char*>(bytes.data()), bytes.size(), encoding.codepage) != utf8)
            throw std::runtime_error("文本包含 " + encoding.name() + " 编码无法表示的字符，请另存为 UTF-8。");
        out.assign(bytes.begin(), bytes.end());
        return out;
    }
    }
    throw std::logic_error("unknown encoding");
}

std::uint64_t fnv1a(const unsigned char* data, std::size_t size)
{
    std::uint64_t h = 1469598103934665603ull;
    for (std::size_t i = 0; i < size; ++i) { h ^= data[i]; h *= 1099511628211ull; }
    return h;
}

} // namespace openmse::core
