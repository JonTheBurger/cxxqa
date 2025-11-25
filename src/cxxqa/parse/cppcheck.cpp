/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std
#include <cassert>
#include <unordered_set>

// 3rd

// local
#include <cxxqa/parse/cppcheck.hpp>
#include <cxxqa/util/log.hpp>
#include <cxxqa/util/ranges.hpp>
#include <cxxqa/util/xml.hpp>

// namespace
namespace cxxqa::cppcheck {
using namespace std::string_view_literals;

/* Functions
 ******************************************************************************/
auto parse_severity(std::string_view severity) noexcept -> Severity
{
  if (severity == "error")
  {
    return Severity::ERROR;
  }
  if (severity == "warning")
  {
    return Severity::WARNING;
  }
  if (severity == "style")
  {
    return Severity::STYLE;
  }
  if (severity == "performance")
  {
    return Severity::PERFORMANCE;
  }
  if (severity == "information")
  {
    return Severity::INFORMATION;
  }

  return Severity::UNKNOWN;
}

auto Location::from(const xml::Node& node) -> Location
{
  assert(node.name() == "location"sv);

  return {
    .file   = node.attribute("file").value(),
    .info   = node.attribute("info").value(),
    .line   = node.attribute("line").as_uint(0),
    .column = node.attribute("column").as_uint(0),
  };
}

auto Error::from(const xml::Node& node) -> Error
{
  assert(node.name() == "error"sv);

  return {
    .id      = node.attribute("id").value(),
    .msg     = node.attribute("msg").value(),
    .verbose = node.attribute("verbose").value(),
    // clang-format off
    .locations = node.children()
      | view::filter([](auto& e) -> auto { return e.name() == "location"sv; })
      | view::transform([](auto& e) -> auto { return Location::from(e); })
      | range::to<std::vector>(),
    // clang-format on
    .symbol       = node.child("symbol").value(),
    .file0        = node.attribute("file0").value(),
    .remark       = node.attribute("remark").value(),
    .cwe          = node.attribute("inconclusive").as_uint(0),
    .inconclusive = node.attribute("inconclusive").as_bool(false),
    .severity     = parse_severity(node.attribute("severity").value()),
  };
}

auto Report::from(const fs::path& xml) -> Result<Report>
{
  xml::Document doc;
  Status        status = doc.load_file(xml.c_str()).status;
  if (!status.success())
  {
    return status;
  }

  auto results = doc.child("results");
  if (results.empty())
  {
    return Code::MISSING_ELEMENT_RESULTS;
  }
  if (results.attribute("version").value() != "2"sv)
  {
    return Code::UNRECOGNIZED_VERSION;
  }
  auto errors = results.child("errors");

#if ((!defined(NDEBUG)) || (!NDEBUG))
  // clang-format off
  for (const auto& unrecognized : errors
    | view::transform([](auto& e) -> std::string_view { return e.name(); })
    | view::filter([](auto str) -> auto { return str != "error"sv; })
    | range::to<std::unordered_set>())
  // clang-format on
  {
    log::warn("Unrecognized XML element(s) found within <errors>: <{}>", unrecognized);
  }
#endif

  return Report{
    // clang-format off
    .errors = errors.children()
      | view::filter([](auto& e) -> auto { return e.name() == "error"sv; })
      | view::transform([](auto& e) -> auto { return Error::from(e); })
      | range::to<std::vector>(),
    // clang-format on
    .version = 2,
  };
}

}  // namespace cxxqa::cppcheck
