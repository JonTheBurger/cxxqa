#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace cxxqa {
class Git {
public:
  Git();

  enum LsFiles : uint8_t {
    LS_DEFAULT  = 0U,
    LS_CACHED   = 1U << 0U,
    LS_DELETED  = 1U << 1U,
    LS_MODIFIED = 1U << 2U,
    LS_OTHER    = 1U << 3U,
    LS_IGNORED  = 1U << 4U,
    LS_STAGED   = 1U << 5U,
    LS_UNMERGED = 1U << 6U,
    LS_KILLED   = 1U << 7U,
  };

  auto ls_files(LsFiles mode = LS_DEFAULT) -> std::vector<std::string>;

  auto exe() const noexcept -> const std::string&;
  auto pwd() const noexcept -> const std::string&;

  auto set_exe(std::string exe) noexcept -> bool;
  auto set_pwd(std::string pwd) noexcept -> bool;

private:
  std::string _exe;
  std::string _pwd;
};
}  // namespace cxxqa
