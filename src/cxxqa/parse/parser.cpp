#include <cxxqa/parse/parser.hpp>

namespace {

constexpr auto skip_space_tab(const char* iter, const char* const end) noexcept -> const char*
{
  while (iter <= end && (*iter == ' ' || *iter == '\t'))
  {
    ++iter;
  }
  return iter;
}

constexpr auto skip_space_tab(std::string_view str, size_t pos) noexcept -> size_t
{
  while (pos < str.size() && (str[pos] == ' ' || str[pos] == '\t'))
  {
    ++pos;
  }
  return pos;
}

}  // namespace

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

auto Parser::to_eof() noexcept -> Parser&
{
  _options |= TO_EOF;
  return *this;
}

auto Parser::until(std::string_view delimiter) noexcept -> Parser&
{
  _options |= UNTIL;
  _delimiter = delimiter;
  return *this;
}

auto Parser::until_passing(std::string_view delimiter) noexcept -> Parser&
{
  _options |= PASS;
  return until(delimiter);
}

auto Parser::through(std::string_view delimiter) noexcept -> Parser&
{
  _delimiter = delimiter;
  return *this;
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

auto Parser::consume_uint(uint8_t radix) -> std::optional<uint32_t>
{
  consume_options();
  return {};
}

auto Parser::consume_int(uint8_t radix) -> std::optional<int32_t>
{
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

  bool found = false;
  if (_delimiter.empty())
  {
    _delimiter = { "\0", 1 };
    found = true;
  }

  const char* it = _source.begin() + _pos;

  if ((_options & Options::LTRIM) != 0U)
  {
    it = skip_space_tab(it, _source.end());
  }

  while (it < _source.end())
  {
    if (*it == _delimiter[0])
    {
      found = true;
      if ((_options & Options::UNTIL) == 0U)
      {
        ++it;
      }
      break;
    }

    if (accept_char(*it))
    {
      ++it;
    }
    else
    {
      break;
    }
  }

  const auto start = _pos;
  _pos             = it - _source.begin();
  auto str         = std::string_view{};

  if (found)
  {
    str = _source.substr(start, _pos - start);
    if ((_options & PASS) != 0U)
    {
      ++_pos;
    }
  }

  if (!found && ((_options & BACKTRACK) != 0U))
  {
    _pos = start;
  }
  else if ((_options & RTRIM) != 0U)
  {
    _pos = skip_space_tab(_source, _pos);
  }

  consume_options();
  if (!found)
  {
    return std::nullopt;
  }
  return str;
}

auto Parser::pos() const noexcept -> size_t
{
  return _pos;
}

auto Parser::string() const noexcept -> std::string_view
{
  return _source.substr(_pos);
}

auto Parser::consume_options() noexcept -> void
{
  _delimiter = {};
  _options   = { Options::DEFAULT };
}

}  // namespace cxxqa
