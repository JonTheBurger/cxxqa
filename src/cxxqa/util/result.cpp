#include <cxxqa/util/result.hpp>

auto fmt::formatter<cxxqa::ErrorMessage>::format(const cxxqa::ErrorMessage& self, format_context& ctx) const -> format_context::iterator
{
  return formatter<std::string_view>::format({ self.c_str(), self.size() }, ctx);
}
