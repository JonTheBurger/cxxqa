/** @file
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <string>
#include <vector>

// 3rd

// local

// namespace
namespace sharif {

/* Types
 ******************************************************************************/
class Config {
public:
  Config();
  static auto from_cli(int argc, char** argv) -> Config;

  auto exclude_matches(std::string_view str) const noexcept -> bool;

  auto include() const noexcept -> const std::vector<std::string>&;
  auto exclude() const noexcept -> const std::vector<std::string>&;
  auto output() const noexcept -> const std::string&;
  auto log_file() const noexcept -> const std::string&;
  auto project() const noexcept -> const std::string&;
  auto preset() const noexcept -> const std::string&;
  auto verbosity() const noexcept -> unsigned;

  struct Troubleshoot {
    bool config;
    bool config_file;
    bool files;
  };
  auto troubleshoot() const noexcept -> const Troubleshoot&;

private:
  std::vector<std::string> _include;
  std::vector<std::string> _exclude;
  std::string _output;
  std::string _log_file;
  std::string _project;
  std::string _preset;
  unsigned    _verbosity;

  Troubleshoot _troubleshoot{};
};

}  // namespace sharif
