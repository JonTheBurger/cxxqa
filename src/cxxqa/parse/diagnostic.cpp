#include <fmt/format.h>

#include <cxxqa/parse/diagnostic.hpp>
#include <cxxqa/parse/parser.hpp>

namespace cxxqa {
auto Diagnostic::from_string(std::string_view str) -> std::optional<Diagnostic>
{
  bool ok = true;
  auto parse = Parser(str);

  // parse.until_eof_or(":").consume_str();
 
  // if (ok) {
  //   auto result = parser.parse<std::string>().until_eof_or(':');
  //   parser.skip_while(':');
  // }
  // if (ok) {
  //   auto result = parser.consume<uint32_t>.until_eof_or(':');
  //   parser.skip_while(':');
  // }
  // if (ok) {
  //   auto result = parser.consume<uint32_t>.until_eof_or(':');
  //   parser.skip_while(':');
  // }



  return {};
}
}  // namespace cxxqa

auto fmt::formatter<cxxqa::Diagnostic>::format(const cxxqa::Diagnostic& self, format_context& ctx) const -> format_context::iterator
{
  return formatter<std::string_view>::format("", ctx);
}
