#include <cxxqa/compile_command.hpp>

#include <glaze/glaze.hpp>

namespace cxxqa {
namespace json = glz;
auto CompileCommand::to_string() const -> std::string
{
  return json::write_json(*this).value_or("ligma");
}

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
}  // namespace cxxqa
