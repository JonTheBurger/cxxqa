/** @file
Error propagation machinery. In summary:

- `Code` is `std::errc`, a generic list of POSIX error codes. These can be used for
  generic comparisons.
- `Status` is a type-erased enum+domain. For example, `Code` is the `std::errc` `enum`
  and the "Generic" domain. An XML parser may use its own enumeration in an "XML"
  domain. So long as the `enum` can fit in a `uintptr_t`, it can fit in `Status`.
- `Result<T>` can be either a `T` or a `Status` on failure. It can be constructed using
  the `ok()` and `err()` helper functions.
- `Outcome<T>` is a `Result<T>` that can also marshal an exception.
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std

// 3rd
#include <boost/outcome/experimental/status_outcome.hpp>
#include <boost/outcome/experimental/status_result.hpp>

// local
#include <cxxqa/util/fmt.hpp>

// namespace
namespace cxxqa {

/// @see https://ned14.github.io/outcome/experimental/status_result/
namespace errors = BOOST_OUTCOME_V2_NAMESPACE::experimental;

/* Defines
 ******************************************************************************/
#define CXXQA_ERROR_NAMESPACE              BOOST_OUTCOME_SYSTEM_ERROR2_NAMESPACE
#define CXXQA_TRY(declaration, expression) OUTCOME_TRY(declaration, expression)

/* Constants
 ******************************************************************************/
/// Used to create a successful `Status`.
inline constexpr errors::errc Success = errors::errc::success;

/* Types
 ******************************************************************************/
/// Error or success + the domain of the error (Type-erased; move-only)
/// @warning This type should not be default constructed.
/// It should be constructed from a `Code::...` or user error type.
using Status = errors::erased_status_code<uintptr_t>;

/// Error codes for type `GenericError`.
using Code = errors::errc;

/// @see https://ned14.github.io/outcome/reference/types/basic_result/
template <typename T, typename E = errors::error>
using Result = errors::status_result<T, E>;

/// @see https://ned14.github.io/outcome/reference/types/basic_outcome/
template <typename T, typename E = errors::error, typename EP = std::exception_ptr>
using Outcome = errors::status_outcome<T, E, EP>;

/// Polymorphic type that can be either a static or heap allocated string
/// @see https://ned14.github.io/outcome/experimental/worked-example-long/string_ref/
using ErrorMessage = system_error2::status_code_domain::string_ref;

/// Use for non-OS specific errors. This uses the `std::errc`, which uses the posix codes.
using GenericError = errors::generic_code;

/// OS-specific `Status`. Use this when calling OS APIs!
using SystemError = errors::system_code;

/// POSIX-specific `Status` - only use this for POSIX APIs!
/// @note Use `PosixError::current()` to retrieve `errno()` as a Status
using PosixError = errors::posix_code;

/// Type for creating custom for specializing error type for use in Status
/// @see https://ned14.github.io/outcome/experimental/worked-example/
template <typename Domain>
using AbstractError = errors::error::status_code<Domain>;

/* Functions
 ******************************************************************************/
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

}  // namespace cxxqa

/// Format ErrorMessage
template <>
struct cxxqa::fmt::formatter<cxxqa::ErrorMessage> : formatter<std::string_view> {
  auto format(const cxxqa::ErrorMessage& self, format_context& ctx) const -> format_context::iterator;
};
