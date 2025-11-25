/** @file
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <algorithm>
#include <ranges>
#include <vector>

// 3rd
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <spdlog/spdlog.h>

// local
#include <cxxqa/tool/git.hpp>
#include <cxxqa/util/proc.hpp>


// namespace
namespace cxxqa {

/* Functions
 ******************************************************************************/
// TODO: don't re-create Process
Git::Git() = default;

auto Git::get_repo_files(const std::vector<std::string>& patterns) -> std::vector<std::string>
{
  Process                  git("git");
  std::vector<std::string> files;

  // Get unstaged files & files in index
  std::vector<std::string> args{
    "ls-files", "--deduplicate", "--other", "--cached", "--exclude-standard", "--"
  };
  args.append_range(patterns);
  git.with_args(std::move(args));
  git.on_stdout(
    [](void* pfiles, std::string_view lines) {
      auto* files = static_cast<std::vector<std::string>*>(pfiles);
      for (const auto& range : std::views::split(lines, '\n'))
      {
        files->emplace_back(range.begin(), range.end());
      }
    },
    &files
  );
  git.run();

  spdlog::info("{}", files);

  // Remove any deleted files
  std::vector<std::string> dargs{
    "ls-files", "--deleted", "--"
  };
  dargs.append_range(patterns);
  git.with_args(std::move(dargs));
  git.on_stdout(
    [](void* pfiles, std::string_view lines) {
      auto* files = static_cast<std::vector<std::string>*>(pfiles);
      for (const auto& range : std::views::split(lines, '\n'))
      {
        auto file = std::string_view{ range };
        auto it   = std::ranges::lower_bound(*files, file);
        if (*it == file)
        {
          files->erase(it);
        }
      }
    },
    &files
  );
  git.run();

  return files;
}

auto Git::root_dir() noexcept -> std::string
{
  auto git = Process{ "git" };
  git.with_args({ "rev-parse", "--show-toplevel" });
  std::string dir;
  git.on_stdout([](void* ptr, std::string_view lines) {
    auto* dir = static_cast<std::string*>(ptr);
    *dir      = lines;
  },
                static_cast<void*>(&dir));
  git.run();

  return dir;
}

auto Git::exe() const noexcept -> const std::string&
{
  return _exe;
}

auto Git::pwd() const noexcept -> const std::string&
{
  return _pwd;
}

auto Git::set_exe(std::string exe) noexcept -> bool
{
  _exe = std::move(exe);
  return true;
}

auto Git::set_pwd(std::string pwd) noexcept -> bool
{
  _pwd = std::move(pwd);
  return true;
}

}  // namespace cxxqa
