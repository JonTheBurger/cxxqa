#include <algorithm>
#include <ranges>
#include <string_view>

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <re2/re2.h>
#include <spdlog/spdlog.h>

#include <cxxqa/core/app.hpp>
#include <cxxqa/core/config.hpp>
#include <cxxqa/parse/compile_command.hpp>
#include <cxxqa/tool/git.hpp>
#include <cxxqa/util/filesystem.hpp>
#include <cxxqa/util/proc.hpp>
#include <cxxqa/util/ranges.hpp>

namespace {
auto to_vector(int argc, char** argv) -> std::vector<std::string>
{
  std::vector<std::string> args;
  args.reserve(static_cast<size_t>(argc));
  for (int i = 0; i < argc; ++i)
  {
    args.emplace_back(argv[i]);
  }
  return args;
}
}  // namespace

namespace cxxqa {

struct App::Impl {
  std::vector<std::string> args;
  std::vector<char*>       argv;

  Config                   config;
  std::vector<std::string> files;

  struct Cache {
    fs::path project_dir;
  } cache;
};

App::App()
  : App(std::vector<std::string>{})
{
}

App::App(int argc, char** argv)
  : App(to_vector(argc, argv))
{
}

App::App(std::vector<std::string> args)
  : _self{ std::make_unique<Impl>() }
{
  set_args(std::move(args));
}

App::~App() = default;

auto App::set_args(std::vector<std::string> args) -> void
{
  _self->args = std::move(args);
  _self->argv.resize(_self->args.size() + 1);
  for (size_t i = 0; i < _self->args.size(); ++i)
  {
    _self->argv[i] = _self->args[i].data();
  }
  _self->argv.back() = nullptr;
}

auto App::argc() const noexcept -> int
{
  const int argc = static_cast<int>(_self->argv.size()) - 1;
  return std::max(argc, 0);
}

auto App::argv() noexcept -> char**
{
  return (_self->argv.size() > 0) ? (_self->argv.data()) : (nullptr);
}

auto App::exec() -> int
{
  _self->config = Config::from_cli(argc(), argv());
  auto git      = Git{};

  _self->files = git.get_repo_files(_self->config.include());
  if (_self->config.troubleshoot().files)
  {
    fmt::println("{}", fmt::join(_self->files, "\n"));
  }

  // fmt::println("{}", git.root_dir());
  auto commands =
    CompileCommand::from_file("build/debug/compile_commands.json") | view::filter([this](const auto& cmd) {
      return !_self->config.exclude_matches(cmd.file);
    }) |
    view::transform([this](const auto& cmd) {
      return cmd.file_as_path(&project_dir());
    }) |
    range::to<std::vector>();

  fmt::println("{}", commands);

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

auto App::project_dir() -> const fs::path&
{
  if (_self->cache.project_dir.empty())
  {
    _self->cache.project_dir = Git{}.root_dir();
  }

  return _self->cache.project_dir;
}

}  // namespace cxxqa
