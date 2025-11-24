#include <cstdio>
#include <exception>
#include <expected>
#include <format>
#include <system_error>
#include <boost/system.hpp>
#include <boost/system/result.hpp>
#include <cxxqa/core/app.hpp>
#include <fmt/format.h>
#include <cxxqa/util/result.hpp>

auto main(int argc, char** argv) -> int
{
  using namespace cxxqa;

  Status status = Code::success;
  fmt::println("{}", status.success());
  fmt::println("{}", status.failure());

  // auto e = std::expected<int, std::error_code>{};
  // fmt::println("{}", sizeof(e));

  // try
  // {
  //   return cxxqa::App{argc, argv}.exec();
  // }
  // catch (const std::exception& ex)
  // {
  //   std::puts(ex.what());
  // }

  return 0;
}
