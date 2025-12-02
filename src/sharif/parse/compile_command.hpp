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
#include <sharif/util/fmt.hpp>

// namespace
namespace sharif {

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

}  // namespace sharif

template <>
struct sharif::fmt::formatter<sharif::CompileCommand> : formatter<std::string_view> {
  auto format(const sharif::CompileCommand& self, format_context& ctx) const -> format_context::iterator;
};
