#pragma once

#include <string>
#include <vector>

namespace cxxqa {
class Config {
public:
  Config();
  static auto from_cli(int argc, char** argv) -> Config;

  auto verbosity() const noexcept -> unsigned;
  auto log_file() const noexcept -> const std::string&;
  auto compile_commands_json() const noexcept -> const std::string&;
  auto preset() const noexcept -> const std::string&;

private:
  unsigned    _verbosity;
  std::string _log_file;
  std::string _output;
  std::vector<std::string> _excludes;

  std::string _compile_commands_json;
  std::string _preset;
};
}  // namespace cxxqa
