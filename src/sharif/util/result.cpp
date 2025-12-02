/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std

// 3rd

// local
#include <sharif/util/result.hpp>

/* Functions
 ******************************************************************************/
auto fmt::formatter<sharif::ErrorMessage>::format(const sharif::ErrorMessage& self, format_context& ctx) const -> format_context::iterator
{
  return formatter<std::string_view>::format({ self.c_str(), self.size() }, ctx);
}
