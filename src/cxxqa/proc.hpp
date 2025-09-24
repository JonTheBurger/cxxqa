#pragma once

#include <flat_map>
#include <memory>
#include <string>
#include <vector>

namespace cxxqa {

class Process {
public:
  using on_output = void(*)(void* context, std::string_view line);

  explicit Process(std::string_view executable);
  ~Process();
  auto with_args(std::vector<std::string> arguments) -> Process&;
  auto with_env(std::flat_map<std::string, std::string> environment) -> Process&;
  auto with_pwd(std::string directory) -> Process&;
  auto on_stdout(on_output callback, void* context = nullptr) -> void;
  auto on_stderr(on_output callback, void* context = nullptr) -> void;
  auto execute() -> int32_t;

private:
  struct Impl;
  std::unique_ptr<Impl> _self;
};

}  // namespace cxxqa
