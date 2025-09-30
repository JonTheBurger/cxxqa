#include <cstdio>
#include <exception>
#include <cxxqa/app.hpp>

auto main(int argc, char** argv) -> int
{
  try
  {
    cxxqa::exec(argc, argv);
  }
  catch (const std::exception& ex)
  {
    std::puts(ex.what());
  }

  return 0;
}
