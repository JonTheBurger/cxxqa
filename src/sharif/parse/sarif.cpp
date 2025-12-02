/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std

// 3rd

// local
#include <sharif/parse/sarif.hpp>
#include <sharif/util/json.hpp>

// using

// namespace
namespace sharif {

/* Functions
 ******************************************************************************/
auto Sarif::to_string() const -> std::string
{
  return json::write_json(*this).value_or("");
}

}  // namespace sharif

auto fmt::formatter<sharif::Sarif>::format(const sharif::Sarif& self, format_context& ctx) const -> format_context::iterator
{
  return fmt::format_to(ctx.out(), "{}", self.to_string());
}
