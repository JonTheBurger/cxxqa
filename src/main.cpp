#include <cstdio>
#include <exception>
#include <expected>
#include <format>
#include <system_error>
#include <boost/system.hpp>
#include <boost/system/result.hpp>
#include <cxxqa/core/app.hpp>
#include <fmt/format.h>
// #include <boost/outcome/experimental/status-code/sys

namespace sys = boost::system;

auto main(int argc, char** argv) -> int
{
  fmt::println("{}", sizeof(sys::error_code{}));
  fmt::println("{}", sizeof(std::error_code{}));

  fmt::println("{}", sizeof(sys::result<int>{}));

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
