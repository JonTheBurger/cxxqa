#pragma once
/**
 * @see https://ned14.github.io/outcome/experimental/status_result/
 */

#include <boost/outcome/experimental/status_outcome.hpp>
#include <boost/outcome/experimental/status_result.hpp>

namespace cxxqa {

/// Namespace for specializing status_code
/// @see https://ned14.github.io/outcome/experimental/worked-example/
namespace errors = BOOST_OUTCOME_V2_NAMESPACE::experimental;

/// @see https://ned14.github.io/outcome/reference/types/basic_result/
template <typename T, typename E = errors::error>
using Result = errors::status_result<T, E>;

/// @see https://ned14.github.io/outcome/reference/types/basic_outcome/
template <typename T, typename E = errors::error, typename EP = std::exception_ptr>
using Outcome = errors::status_outcome<T, E, EP>;

}  // namespace cxxqa
