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

// local
#include <cxxqa/util/fmt.hpp>

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
struct cxxqa::fmt::formatter<cxxqa::CompileCommand> : formatter<std::string_view> {
  auto format(const cxxqa::CompileCommand& self, format_context& ctx) const -> format_context::iterator;
};
