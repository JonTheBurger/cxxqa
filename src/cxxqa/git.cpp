#include <algorithm>
#include <ranges>
#include <vector>

#include <fmt/base.h>
#include <fmt/format.h>

#include <cxxqa/git.hpp>
#include <cxxqa/proc.hpp>

namespace cxxqa {

Git::Git() = default;

auto Git::get_repo_files(const std::vector<std::string>& extensions) -> std::vector<std::string>
{
  Process                  git("git");
  std::vector<std::string> files;

  // Get unstaged files & files in index
  std::vector<std::string> args{
    "ls-files", "--deduplicate", "--other", "--cached", "--exclude-standard", "--"
  };
  args.append_range(extensions);
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
  git.execute();

  // Remove any deleted files
  std::vector<std::string> dargs{
    "ls-files", "--deleted", "--"
  };
  dargs.append_range(extensions);
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
  git.execute();

  return files;
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
