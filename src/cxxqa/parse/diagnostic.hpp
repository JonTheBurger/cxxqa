#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cxxqa {
// test-labelled-ranges.c:9:6: error: mismatching types: 'int' and 'const char *'
// https://gcc.gnu.org/wiki/libgdiagnostics
struct Diagnostic {
  std::string file;
  uint32_t    line;
  uint32_t    column;
  std::string severity;
  std::string message;
  std::string category;
  std::string source;

  static auto from_string(std::string_view str) -> std::optional<Diagnostic>;
};
}  // namespace cxxqa

#include <fmt/base.h>
template <>
struct fmt::formatter<cxxqa::Diagnostic> : formatter<std::string_view> {
  auto format(const cxxqa::Diagnostic& self, format_context& ctx) const -> format_context::iterator;
};
