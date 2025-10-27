#include <optional>
#include <string_view>
#include <cxxqa/parse/parser.hpp>

namespace cxxqa {

Parser::Parser(std::string_view source)
  : _source{ source }
{
}

auto Parser::or_backtrack() noexcept -> Parser&
{
  _options |= BACKTRACK;
  return *this;
}

auto Parser::to_newline() noexcept -> Parser&
{
  _options |= TO_CRLF;
  return *this;
}

auto Parser::to_eof() noexcept -> Parser&
{
  _options |= TO_EOF;
  return *this;
}

auto Parser::until(std::string_view delimiter) noexcept -> Parser&
{
  _delimiter = delimiter;
  return *this;
}

auto Parser::until_and_past(std::string_view delimiter) noexcept -> Parser&
{
  _options |= PASS;
  _options &= ~INCLUDE;
  return until(delimiter);
}

auto Parser::until_and_including(std::string_view delimiter) noexcept -> Parser&
{
  _options |= INCLUDE;
  _options &= ~PASS;
  return until(delimiter);
}

auto Parser::and_ltrim() noexcept -> Parser&
{
  _options |= LTRIM;
  return *this;
}

auto Parser::and_rtrim() noexcept -> Parser&
{
  _options |= RTRIM;
  return *this;
}

auto Parser::and_trim() noexcept -> Parser&
{
  _options |= LTRIM;
  _options |= RTRIM;
  return *this;
}

auto Parser::consume_uint() -> std::optional<uint32_t>
{
  if (auto result = consume_str(is_decimal); result)
  {
    uint32_t value{};
    auto     str = *result;

    auto [ptr, err] = std::from_chars(str.data(), str.data() + str.size(), value);
    if (err == std::errc{} && ptr == str.data() + str.size())
    {
      return value;
    }
  }
  return std::nullopt;
}

auto Parser::consume_int(uint8_t radix) -> std::optional<int32_t>
{
  // TODO: We don't accept '-'
  check_char accept_digit = [radix]() -> check_char {
    switch (radix)
    {
      case 2:
        return cxxqa::is_bin;
      case 8:
        return cxxqa::is_octal;
      case 16:
        return cxxqa::is_hex;
      default:
        return cxxqa::is_decimal;
    }
  }();

  if (auto result = consume_str(accept_digit); result)
  {
    int32_t value{};
    auto    str = *result;

    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
    if (ec == std::errc{} && ptr == str.data() + str.size())
    {
      return value;
    }
  }
  return std::nullopt;
}

auto Parser::consume_str(check_char accept_char) -> std::optional<std::string_view>
{
  if (_pos >= _source.size())
  {
    return std::nullopt;
  }

  if (_delimiter.empty())
  {
    _delimiter = { "\0", 1 };
  }

  if ((_options & Options::LTRIM) != 0U)
  {
    skip_space_tab();
  }

  const auto reset  = _pos;
  size_t     start = _pos;
  size_t     end   = _pos;
  bool       found  = true;
  while (_pos < _source.size())
  {
    if (string().starts_with(_delimiter))
    {
      end = _pos;
      if ((_options & Options::PASS) != 0U)
      {
        _pos += _delimiter.length();
      }
      else if ((_options & Options::INCLUDE) != 0U)
      {
        _pos += _delimiter.length();
        end += _delimiter.length();
      }
      break;
    }

    if ((_options & Options::TO_CRLF) != 0U)
    {
      if (peek() == '\n')
      {
        end = _pos;
        ++_pos;
        break;
      }

      if (peek() == '\r')
      {
        end = _pos;
        ++_pos;
        if (peek() == '\n')
        {
          ++_pos;
        }
        break;
      }
    }

    if (accept_char(peek()))
    {
      ++_pos;
      ++end;
    }
    else
    {
      break;
      found = false;
    }
  }

  if (!found && ((_options & BACKTRACK) != 0U))
  {
    _pos = reset;
  }
  else if ((_options & RTRIM) != 0U)
  {
    skip_space_tab();
  }

  reset_options();
  return (found) ? (std::optional{ _source.substr(start, end - start) }) : (std::nullopt);
}

auto Parser::skip(uint32_t n) noexcept -> void
{
  _pos = std::min(_pos + n, _source.size());
}

auto Parser::skip_while(check_char skip_when) noexcept -> void
{
  while (_pos < _source.size() && skip_when(_source[_pos]))
  {
    ++_pos;
  }
}

auto Parser::peek() const noexcept -> char
{
  return (_pos < _source.size()) ? _source[_pos] : '\0';
}

auto Parser::pos() const noexcept -> size_t
{
  return _pos;
}

auto Parser::string() const noexcept -> std::string_view
{
  return _source.substr(_pos);
}

auto Parser::reset_options() noexcept -> void
{
  _delimiter = {};
  _options   = { Options::DEFAULT };
}

auto Parser::skip_space_tab() noexcept -> void
{
  while (_pos < _source.size() && (_source[_pos] == ' ' || _source[_pos] == '\t'))
  {
    ++_pos;
  }
}

}  // namespace cxxqa
