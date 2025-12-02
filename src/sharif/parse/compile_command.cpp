/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std

// 3rd

// local
#include <sharif/parse/compile_command.hpp>
#include <sharif/util/filesystem.hpp>
#include <sharif/util/json.hpp>

// namespace
namespace sharif {

/* Functions
 ******************************************************************************/
auto CompileCommand::from_file(std::string_view file) -> std::vector<CompileCommand>
{
  std::vector<CompileCommand> compile_commands;
  json::read_file_json(compile_commands, file, std::string{});
  return compile_commands;
}

auto CompileCommand::to_file(std::string_view file, const std::vector<CompileCommand>& commands) -> void
{
  json::write_file_json(commands, file, std::string{});
}

auto CompileCommand::dir_as_path(const std::filesystem::path* relative_to) const -> std::filesystem::path
{
  if (!relative_to)
  {
    return std::filesystem::path{ directory };
  }
  return std::filesystem::relative(directory, *relative_to);
}

auto CompileCommand::cmd_as_vec() const -> std::vector<std::string>
{
  // TODO: Implement this

// "/usr/lib64/ccache/c++
// -DBOOST_CONTAINER_NO_LIB
// -DBOOST_CONTAINER_STATIC_LINK
// -DBOOST_CONTEXT_EXPORT=\"\"
// -DBOOST_CONTEXT_NO_LIB=\"\"
// -DBOOST_CONTEXT_STATIC_LINK=\"\"
// -DBOOST_DATE_TIME_NO_LIB
// -DBOOST_DATE_TIME_STATIC_LINK
// -DBOOST_PROCESS_STATIC_LINK
// -DBOOST_PROCESS_USE_STD_FS=1
// -DCLI11_COMPILE
// -DSPDLOG_COMPILED_LIB
// -DSPDLOG_FMT_EXTERNAL
// -I/home/jon/Projects/cxx/sharif/src
// -isystem
// /home/jon/Projects/cxx/sharif/build/debug/vcpkg_installed/x64-linux/include
// -Wall
// -Wextra
// -Wpedantic
// -g
// -std=gnu++23
// -fdiagnostics-color=always
// -fmodules-ts
// -fmodule-mapper=CMakeFiles/sharif.dir/src/sharif/core/config.cpp.o.modmap
// -MD
// -fdeps-format=p1689r5
// -x
// c++
// -o
// CMakeFiles/sharif.dir/src/sharif/core/config.cpp.o
// -c
// /home/jon/Projects/cxx/sharif/src/sharif/core/config.cpp"
  std::vector<std::string> commands;

  return {};
}

auto CompileCommand::file_as_path(const std::filesystem::path* relative_to) const -> std::filesystem::path
{
  if (!relative_to)
  {
    return std::filesystem::path{ file };
  }
  return std::filesystem::relative(file, *relative_to);
}

auto CompileCommand::to_string() const -> std::string
{
  return json::write_json(*this).value_or("");
}

}  // namespace sharif

auto fmt::formatter<sharif::CompileCommand>::format(const sharif::CompileCommand& self, format_context& ctx) const -> format_context::iterator
{
  return fmt::format_to(ctx.out(), "{}", self.to_string());
}
