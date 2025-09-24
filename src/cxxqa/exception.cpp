#include <cxxqa/exception.hpp>

namespace cxxqa {

Exception::Exception(Error code, std::string message)
  : _message{ std::move(message) }
  , _code{ code }
{
}

auto Exception::code() const noexcept -> Error
{
  return _code;
}

auto Exception::what() const noexcept -> const char*
{
  return _message.c_str();
}

}  // namespace cxxqa
