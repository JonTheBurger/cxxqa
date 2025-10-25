#pragma once

#include <cctype>
#include <charconv>
#include <cstdint>
#include <optional>
#include <string_view>

namespace cxxqa {

constexpr auto is_letter(char chr) noexcept -> bool
{
  return ('A' <= chr && chr <= 'Z') || ('a' <= chr && chr <= 'z');
}

constexpr auto to_lower(char chr) noexcept -> char
{
  return ('A' <= chr && chr <= 'Z') ? static_cast<char>(chr + ('a' - 'A')) : (chr);
}

constexpr auto to_upper(char chr) noexcept -> char
{
  return ('a' <= chr && chr <= 'z') ? static_cast<char>(chr - ('a' - 'A')) : (chr);
}

constexpr auto is_decimal(char chr) noexcept -> bool
{
  return '0' <= chr && chr <= '9';
}

constexpr auto is_bin(char chr) noexcept -> bool
{
  return (chr == '0') || (chr == '1') || (chr == 'b') || (chr == 'B');
}

constexpr auto is_octal(char chr) noexcept -> bool
{
  return ('0' <= chr && chr <= '7') || (chr == 'o') || (chr == 'O');
}

constexpr auto is_hex(char chr) noexcept -> bool
{
  return is_decimal(chr) || ('a' <= chr && chr <= 'f') || ('A' <= chr && chr <= 'F') || ('x' == chr) || ('X' == chr);
}

constexpr auto accept_any_char(char /*chr*/) noexcept -> bool
{
  return true;
}

using check_char = bool(&)(char);

class Parser {
public:
  explicit Parser(std::string_view source);

  enum Options {
    DEFAULT = 0,
    /// Move the cursor back if the parser does not find a match
    BACKTRACK = 1 << 0,
    /// Allow EOF instead of the specified delimiter
    TO_EOF = 1 << 1,
    /// Parse until the delimiter is found, excluding the delimiter
    UNTIL = 1 << 2,
    /// Parse until the delimiter is found, and pass it
    PASS = 1 << 3,
    /// Skip spaces and tabs before parsing
    LTRIM = 1 << 4,
    /// Skip spaces and tabs after parsing
    RTRIM = 1 << 5,
  };

  /** Do not progress the parser if a match was not found */
  auto or_backtrack() noexcept -> Parser&;

  /** Allow EOF instead of the specified delimiter */
  auto to_eof() noexcept -> Parser&;

  /** Parse string until a delimiter is found, excluding the delimiter. */
  auto until(std::string_view delimiter) noexcept -> Parser&;

  /** Parse string until a delimiter is found, excluding the delimiter, then move past the delimiter */
  auto until_passing(std::string_view delimiter) noexcept -> Parser&;

  /** Parse string until a delimiter is found, include the delimiter. */
  auto through(std::string_view delimiter) noexcept -> Parser&;

  /** Skip any spaces/tabs found before parsing. */
  auto and_ltrim() noexcept -> Parser&;

  /** Skip any spaces/tabs found after parsing. */
  auto and_rtrim() noexcept -> Parser&;

  /** Skip any spaces/tabs found before or after parsing. */
  auto and_trim() noexcept -> Parser&;

  auto consume_uint(uint8_t radix = 10) -> std::optional<uint32_t>;

  auto consume_int(uint8_t radix = 10) -> std::optional<int32_t>;

  auto consume_str(check_char accept_char = accept_any_char) -> std::optional<std::string_view>;

  auto pos() const noexcept -> size_t;

  auto string() const noexcept -> std::string_view;

  auto delimiter() const noexcept -> char;

private:
  auto consume_options() noexcept -> void;

  std::string_view _source;
  std::string_view _delimiter{};
  size_t           _pos{ 0 };
  size_t           _options{ Options::DEFAULT };
};

}  // namespace cxxqa
