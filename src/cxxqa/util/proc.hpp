#pragma once

#include <flat_map>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>

namespace cxxqa {

// TODO: probably switch to std::function or move_only_function
class Process {
public:
  using on_output = void (*)(void* context, std::string_view chunk);

  explicit Process(std::string_view executable);
  Process(std::string_view executable, std::vector<std::string> arguments);
  ~Process();
  auto exe() const noexcept -> std::string_view;
  auto args() const noexcept -> const std::vector<std::string>&;
  auto with_args(std::vector<std::string> arguments) -> Process&;
  auto with_env(std::flat_map<std::string, std::string> environment) -> Process&;
  auto with_pwd(std::string directory) -> Process&;
  auto on_stdout(on_output callback, void* context = nullptr) -> void;
  auto on_stderr(on_output callback, void* context = nullptr) -> void;

  struct ExitCode {
    int32_t exit_code;
  };

  auto run() -> int32_t;

  struct Lines {
    int32_t                  exit_code{};
    std::vector<std::string> stdout;
    std::vector<std::string> stderr;
  };

  auto run_lines() -> Lines;

  struct Text {
    int32_t     exit_code{};
    std::string stdout;
    std::string stderr;
  };

  auto run_text() -> Text;

private:
  struct Impl;
  std::unique_ptr<Impl> _self;
};

}  // namespace cxxqa

template <>
struct fmt::formatter<cxxqa::Process> {
  constexpr auto parse(fmt::format_parse_context& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const cxxqa::Process& self, FormatContext& ctx) const
  {
    auto out = fmt::format_to(ctx.out(), "\"{}\"", self.exe());
    for (const auto& arg : self.args())
    {
      out = fmt::format_to(out, " \"{}\"", arg);
    }
    return out;
  }
};
