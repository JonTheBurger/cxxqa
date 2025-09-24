#include <string_view>

#include <cxxqa/config.hpp>
#include <cxxqa/exception.hpp>
#include <cxxqa/git.hpp>
#include <cxxqa/proc.hpp>

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

auto main(int argc, char** argv) -> int
{
  try
  {
    auto config = cxxqa::Config::from_cli(argc, argv);

    auto proc = cxxqa::Process("tree");
    proc.with_args({"/home"});
    proc.on_stdout([](void*, std::string_view out){
      fmt::println("{}", out);
    });
    proc.execute();

    // auto git = cxxqa::Git{};
    // fmt::println("{}", git.ls_files());
  }
  catch (const cxxqa::Exception& ex)
  {
    fmt::println("{}", ex.what());
  }

  return 0;
}
