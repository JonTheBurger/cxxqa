/** @file
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std

// 3rd

// local
#include <cxxqa/util/filesystem.hpp>
#include <cxxqa/util/result.hpp>
#include <cxxqa/util/xml.hpp>

// namespace
namespace cxxqa::cppcheck {

/* Defines
 ******************************************************************************/

/* Constants
 ******************************************************************************/

/* Types
 ******************************************************************************/
enum class Code : uint8_t {
  OK = 0,
  MISSING_ELEMENT_RESULTS,
  UNRECOGNIZED_VERSION,
};

enum class Severity : uint8_t {
  UNKNOWN = 0,
  INFORMATION,
  PERFORMANCE,
  STYLE,
  WARNING,
  ERROR,
};

struct Location {
  static auto from(const xml::Node& node) -> Location;

  std::string file;         ///< Filename, both relative and absolute paths are possible
  std::string info;         ///< Short information for each location (optional)
  uint32_t    line{ 0 };    ///< Line number (1-based)
  uint32_t    column{ 0 };  ///< Column number (0-based)
};

struct Error {
  static auto from(const xml::Node& node) -> Error;

  std::string           id;       ///< Id of error, and which are valid symbolnames
  std::string           msg;      ///< The error message in short format
  std::string           verbose;  ///< The error message in long format
  std::vector<Location> locations;
  std::string           symbol;                         ///< Name of the symbol that produced the error (optional)
  std::string           file0;                          ///< Name of the source file (optional)
  std::string           remark;                         ///< Optional attribute. The related remark/justification from a remark comment.
  uint32_t              cwe{ 0 };                       ///< CWE ID for the problem; note that this attribute is only used when the CWE ID for the message is known
  bool                  inconclusive{ false };          ///< This attribute is only used when the error message is inconclusive
  Severity              severity{ Severity::UNKNOWN };  ///< error/warning/style/performance/portability/information
};

struct Report {
  static auto from(const fs::path& xml) -> Result<Report>;

  std::vector<Error> errors;
  uint8_t            version;
};

/* Functions
 ******************************************************************************/
auto parse_severity(std::string_view severity) noexcept -> Severity;

};  // namespace cxxqa::cppcheck

namespace CXXQA_ERROR_NAMESPACE {
// NOLINTBEGIN(readability-identifier-naming)
template <>
struct quick_status_code_from_enum<cxxqa::cppcheck::Code> : quick_status_code_from_enum_defaults<cxxqa::cppcheck::Code> {
  static constexpr const auto domain_name = "cppcheck xml";
  /// https://www.random.org/cgi-bin/randbyte?nbytes=16&format=h
  static constexpr const auto domain_uuid = "{445e1427-793d-62d4-e0de-dd9a6beeb0cc}";

  static auto value_mappings() -> const std::initializer_list<mapping>&
  {
    using enum cxxqa::cppcheck::Code;
    static const std::initializer_list<mapping> errors = {
      { .value = MISSING_ELEMENT_RESULTS, .message = "Report missing required element <results>", .code_mappings = { cxxqa::Code::bad_message } },
      { .value = UNRECOGNIZED_VERSION, .message = "<results> version attribute has an unrecognized value", .code_mappings = { cxxqa::Code::bad_message } },
      // { .value = cxxqa::Error::, .message = "", .code_mappings = { cxxqa::Code::bad_message } },
    };
    return errors;
  }
};

// NOLINTEND(readability-identifier-naming)
}  // namespace CXXQA_ERROR_NAMESPACE
