/** @file
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

// 3rd
#include <fmt/format.h>

// local

// namespace
namespace cxxqa {

/* Types
 ******************************************************************************/
struct CompileCommand {
  static auto from_file(std::string_view file) -> std::vector<CompileCommand>;
  static auto to_file(std::string_view file, const std::vector<CompileCommand>& commands) -> void;

  std::string directory;
  std::string command;
  std::string file;
  std::string output;

  auto dir_as_path(const std::filesystem::path* relative_to = nullptr) const -> std::filesystem::path;
  auto cmd_as_vec() const -> std::vector<std::string>;
  auto file_as_path(const std::filesystem::path* relative_to = nullptr) const -> std::filesystem::path;
  auto to_string() const -> std::string;
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
