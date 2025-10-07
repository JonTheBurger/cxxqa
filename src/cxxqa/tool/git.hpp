#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace cxxqa {
class Git {
public:
  Git();

  enum class Submodules {
    IGNORE  = 0,
    RECURSE = 1,
  };

  // TODO: make these std::filesystem::paths
  auto get_repo_files(const std::vector<std::string>& patterns) -> std::vector<std::string>;
  auto root_dir() noexcept -> std::string;

  auto exe() const noexcept -> const std::string&;
  auto pwd() const noexcept -> const std::string&;

  auto set_exe(std::string exe) noexcept -> bool;
  auto set_pwd(std::string pwd) noexcept -> bool;

  // TODO: something to get just changed files
  // TODO: precommit hook

private:
  std::string _exe;
  std::string _pwd;
};
}  // namespace cxxqa
