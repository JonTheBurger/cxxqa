#include <cxxqa/core/exception.hpp>

namespace cxxqa {

Exception::Exception(Error code, const std::string& message)
  : std::runtime_error(message)
  , _code{ code }
{
}

auto Exception::code() const noexcept -> Error
{
  return _code;
}

}  // namespace cxxqa
