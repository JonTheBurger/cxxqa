#include <string_view>

#include <cxxqa/compile_command.hpp>
#include <cxxqa/config.hpp>
#include <cxxqa/exception.hpp>
#include <cxxqa/git.hpp>
#include <cxxqa/proc.hpp>

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

namespace cxxqa {
auto exec(int argc, char** argv) -> int
{
  auto config     = Config::from_cli(argc, argv);
  auto git        = Git{};
  auto extensions = std::vector<std::string>{
    "*.cpp",
    "*.hpp",
  };
  fmt::println("{}", git.get_repo_files(extensions));

  auto cc = CompileCommand::from_file("build/debug/compile_commands.json");
  fmt::println("{}", cc);

  // auto proc = cxxqa::Process("tree");
  // proc.with_args({"/home"});
  // proc.on_stdout([](void* ctx, std::string_view out){
  //   const auto* proc = static_cast<cxxqa::Process*>(ctx);
  //   for (const auto& range : std::views::split(out, '\n'))
  //   {
  //     std::string_view line{range};
  //     fmt::println("{}: {}", fmt::styled(proc->exe(), fg(fmt::color::purple)), line);
  //   }
  // }, &proc);
  // proc.execute();

  return 0;
}
}  // namespace cxxqa
