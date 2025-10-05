#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace cxxqa {

// TODO: Make this result<> compatible, maybe std::error_code / std::outcome compatible
enum class Error : std::uint32_t {
  OK = 0,

  PROCESS           = 100,
  PROCESS_NOT_FOUND = 101,

  PARSE                      = 200,
  PARSE_BAD_COMPILE_COMMANDS = 202,
};

// TODO: Make this extend std::system_error for std::error_code?
class Exception : public std::runtime_error {
public:
  Exception(Error code, const std::string& message);
  auto code() const noexcept -> Error;

private:
  Error _code;
};
}  // namespace cxxqa
