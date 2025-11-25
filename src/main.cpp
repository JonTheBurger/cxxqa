#include <exception>

#include <cxxqa/core/app.hpp>
#include <cxxqa/parse/cppcheck.hpp>
#include <cxxqa/util/result.hpp>

auto main(int argc, char** argv) -> int
{
  try
  {
    return cxxqa::App{ argc, argv }.exec();
  }
  catch (const std::exception& ex)
  {
    std::puts(ex.what());
  }

  return 0;
}
