#pragma once

#include <cstdint>
#include <exception>
#include <string>

namespace cxxqa {

enum class Error : std::uint32_t {
  OK = 0,

  PROCESS = 100,
  PROCESS_NOT_FOUND = 101,
};

class Exception : public std::exception {
public:
  Exception(Error code, std::string message);
  auto code() const noexcept -> Error;
  auto what() const noexcept -> const char* override;

private:
  Error         _code;
  std::string   _message;
};
}  // namespace cxxqa
