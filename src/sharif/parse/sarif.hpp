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
#include <sharif/parse/detail/sarif_spec.hpp>
#include <sharif/util/filesystem.hpp>
#include <sharif/util/fmt.hpp>
#include <sharif/util/json.hpp>
#include <sharif/util/result.hpp>

// namespace
namespace sharif {

/* Types
 ******************************************************************************/
class Sarif {
public:
  auto to_string() const -> std::string;

  std::string             version{ "2.1.0" };
  std::vector<sarif::Run> runs;
};

}  // namespace sharif

template <>
struct sharif::fmt::formatter<sharif::Sarif> : formatter<std::string_view> {
  auto format(const sharif::Sarif& self, format_context& ctx) const -> format_context::iterator;
};
