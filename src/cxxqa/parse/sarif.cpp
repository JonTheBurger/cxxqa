/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std

// 3rd

// local
#include <cxxqa/parse/sarif.hpp>
#include <cxxqa/util/json.hpp>

// using

// namespace
namespace cxxqa {

/* Functions
 ******************************************************************************/
auto Sarif::to_string() const -> std::string
{
  return json::write_json(*this).value_or("");
}

}  // namespace cxxqa

auto fmt::formatter<cxxqa::Sarif>::format(const cxxqa::Sarif& self, format_context& ctx) const -> format_context::iterator
{
  return fmt::format_to(ctx.out(), "{}", self.to_string());
}
