#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>

namespace cxxqa {
struct CompileCommand {
  std::string directory;
  std::string command;
  std::string file;
  std::string output;

  auto to_string() const -> std::string;
  static auto from_file(std::string_view file) -> std::vector<CompileCommand>;
  static auto to_file(std::string_view file, const std::vector<CompileCommand>& commands) -> void;
};
}  // namespace cxxqa

template <>
struct fmt::formatter<cxxqa::CompileCommand> {
  constexpr auto parse(fmt::format_parse_context& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const cxxqa::CompileCommand& self, FormatContext& ctx) const
  {
    return fmt::format_to(ctx.out(), "{}", self.to_string());
  }
};
