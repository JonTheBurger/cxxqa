#pragma once
#include <cstdint>

#include <boost/outcome/experimental/status_outcome.hpp>
#include <boost/outcome/experimental/status_result.hpp>

#define CXXQA_ERROR_NAMESPACE_BEGIN namespace BOOST_OUTCOME_SYSTEM_ERROR2_NAMESPACE {
#define CXXQA_ERROR_NAMESPACE_END   }

namespace cxxqa {

/// @see https://ned14.github.io/outcome/experimental/status_result/
namespace errors = BOOST_OUTCOME_V2_NAMESPACE::experimental;

/// Error or success + the domain of the error (Type-erased; move-only)
/// @warning This type should not be default constructed.
/// It should be constructed from a `Code::...` or user error type.
using Status = errors::erased_status_code<uintptr_t>;

/// Error codes for type `GenericError`.
using Code = errors::errc;

/// Used to create a successful `Status`.
inline constexpr Code Success = Code::success;

/// @see https://ned14.github.io/outcome/reference/types/basic_result/
template <typename T, typename E = errors::error>
using Result = errors::status_result<T, E>;

/// Creates a successful Result<T, EC>
template <typename T>
constexpr auto ok(T&& value)
{
  return errors::success(std::forward<T>(value));
}

/// Creates a failed Result<T, EC>
template <typename EC>
auto err(EC&& error)
{
  return errors::failure(std::forward<EC>(error));
}

/// @see https://ned14.github.io/outcome/reference/types/basic_outcome/
template <typename T, typename E = errors::error, typename EP = std::exception_ptr>
using Outcome = errors::status_outcome<T, E, EP>;

/// @see https://ned14.github.io/outcome/experimental/worked-example-long/string_ref/
using ErrorMessage = system_error2::status_code_domain::string_ref;

using GenericError = errors::generic_code;
using SystemError  = errors::system_code;
using PosixError   = errors::posix_code;

/// Type for creating custom for specializing error type for use in Status
/// @see https://ned14.github.io/outcome/experimental/worked-example/
template <typename Domain>
using AbstractError = errors::error::status_code<Domain>;

enum class Error : std::uint32_t {
  OK = 0,
  PROCESS_NOT_FOUND = 101,
  PARSE_BAD_COMPILE_COMMANDS = 202,
};

class Exception : public std::runtime_error {
public:
  Exception(Error code, const std::string& message);
  auto code() const noexcept -> Error;

private:
  Error _code;
};

}  // namespace cxxqa

CXXQA_ERROR_NAMESPACE_BEGIN

template <>
struct quick_status_code_from_enum<cxxqa::Error> : quick_status_code_from_enum_defaults<cxxqa::Error> {
  static constexpr const auto domain_name = "cxxqa";
  static constexpr const auto domain_uuid = "{e2043210-30aa-41f9-56b2-827cd9e2264e}";

  static auto value_mappings() -> const std::initializer_list<mapping>&
  {
    static const std::initializer_list<mapping> errors = {
      { .value = cxxqa::Error::OK, .message = "OK", .code_mappings = { cxxqa::Code::success } },
      { .value = cxxqa::Error::PROCESS_NOT_FOUND, .message = "Process not found", .code_mappings = { cxxqa::Code::no_such_file_or_directory } },
      { .value = cxxqa::Error::PARSE_BAD_COMPILE_COMMANDS, .message = "Invalid compile_commands.json", .code_mappings = {} },
    };
    return errors;
  }

  // Completely optional definition of mixin for the status code synthesised from `Enum`. It can be omitted.
  template <class Base>
  struct mixin : Base {
    using Base::Base;
  };
};

CXXQA_ERROR_NAMESPACE_END

#include <fmt/format.h>

template <>
struct fmt::formatter<cxxqa::ErrorMessage> : formatter<std::string_view> {
  auto format(const cxxqa::ErrorMessage& self, format_context& ctx) const -> format_context::iterator;
};
