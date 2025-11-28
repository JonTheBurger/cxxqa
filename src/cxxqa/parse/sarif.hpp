/** @file
 *
 * Defines structs to serialize the Static Analysis Results Interchange Format (SARIF).
 * This is one of the common input/output formats
 * @see https://www.sarif.info/
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <string>

// 3rd

// local
#include <cxxqa/parse/detail/sarif_spec.hpp>
#include <cxxqa/util/filesystem.hpp>
#include <cxxqa/util/fmt.hpp>
#include <cxxqa/util/json.hpp>
#include <cxxqa/util/result.hpp>

// namespace
namespace cxxqa {

/* Types
 ******************************************************************************/
class Sarif {
public:
  auto to_string() const -> std::string;

  std::string             version{ "2.1.0" };
  std::vector<sarif::Run> runs;
};

}  // namespace cxxqa

template <>
struct cxxqa::fmt::formatter<cxxqa::Sarif> : formatter<std::string_view> {
  auto format(const cxxqa::Sarif& self, format_context& ctx) const -> format_context::iterator;
};
