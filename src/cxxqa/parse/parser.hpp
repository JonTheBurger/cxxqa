#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace cxxqa {

class Parser {
public:
  explicit Parser(std::string_view source)
    : _source{ source }
  {
  }

  enum Options {
    DEFAULT   = 0,
    BACKTRACK = 1 << 0,
    TO_EOF    = 1 << 1,
    UNTIL     = 1 << 2,
    PASS      = 1 << 3,
  };

  /** Do not progress the parser if a match was not found */
  auto or_backtrack() -> Parser&
  {
    _options |= BACKTRACK;
    return *this;
  }

  /** Allow EOF instead of the specified delimiter */
  auto to_eof() -> Parser&
  {
    _options |= TO_EOF;
    return *this;
  }

  /** Parse string until a delimiter is found, excluding the delimiter. */
  auto until(std::string_view delimiter) -> Parser&
  {
    _options |= UNTIL;
    _delimiter = delimiter;
    return *this;
  }

  /** Parse string until a delimiter is found, excluding the delimiter, then move past the delimiter */
  auto until_passing(std::string_view delimiter) -> Parser&
  {
    _options |= PASS;
    return until(delimiter);
  }

  /** Parse string until a delimiter is found, include the delimiter. */
  auto through(std::string_view delimiter) -> Parser&
  {
    _delimiter = delimiter;
    return *this;
  }

  auto consume_uint(uint8_t radix = 10) -> std::optional<uint32_t>
  {
    consume_options();
    return {};
  }

  auto consume_int(uint8_t radix = 10) -> std::optional<int32_t>
  {
    consume_options();
    return {};
  }

  auto consume_str(bool (*while_)(char chr) = nullptr) -> std::optional<std::string_view>
  {
    if (_pos >= _source.size())
    {
      return std::nullopt;
    }
    const auto start = _pos;
    bool       found = false;

    auto it = _source.begin() + _pos;
    while ((it != _source.end()) && (while_ == nullptr || while_(*it)))
    {
      if (*it == _delimiter[0])
      {
        found = true;
        if (!(_options | Options::UNTIL))
        {
          ++it;
        }
        break;
      }
      ++it;
    }

    _pos = it - _source.begin();
    auto str = std::string_view{};
    if (found)
    {
      str = _source.substr(start, _pos - start);
      if (_options | PASS)
      {
        ++_pos;
      }
    }
    if (!found && (_options | BACKTRACK))
    {
      _pos = start;
    }

    consume_options();
    if (!found)
    {
      return std::nullopt;
    }
    return str;
  }

  auto pos() -> size_t
  {
    return _pos;
  }

  auto string() -> std::string_view
  {
    return _source.substr(_pos);
  }

private:
  auto consume_options() -> void
  {
    _delimiter = {};
    _options   = { Options::DEFAULT };
  }

  std::string_view _source;
  std::string_view _delimiter{};
  size_t           _pos{ 0 };
  size_t           _options{ Options::DEFAULT };
};

}  // namespace cxxqa
