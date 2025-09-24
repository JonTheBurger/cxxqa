#include <boost/asio.hpp>
#include <boost/asio/readable_pipe.hpp>
#include <boost/process.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/v2/environment.hpp>
#include <fmt/base.h>
#include <fmt/format.h>

#include <cxxqa/git.hpp>

namespace asio = boost::asio;
namespace proc = boost::process;
namespace sys = boost::system;

namespace cxxqa {

Git::Git()
{
  // auto git = proc::search_path("git");
  // if (!git.empty())
  // {
  //   _exe = git;
  // }
  _exe = "/usr/bin/git";
}

auto Git::ls_files(LsFiles mode) -> std::vector<std::string>
{
  asio::io_context ctx;
  asio::readable_pipe stdout_pipe{ctx};
  proc::process git(ctx, _exe, {"ls-files"}, proc::process_stdio{{}, stdout_pipe, {}});

  std::vector<std::string> files;

  while (git.running()) {
      std::string line;
      line.reserve(1024);
      sys::error_code ec;
      asio::read_until(stdout_pipe, asio::dynamic_buffer(line), '\n', ec);

      if (!ec && !line.empty()) {
          // Process the line (remove trailing newline if needed)
          if (!line.empty() && line.back() == '\n') {
              line.pop_back();
          }
          fmt::println("{}", line);
          line.clear();
      }
  }

  // auto git = proc::child(_exe, proc::std_out > pipe_stream);

  // std::string    line;

  // while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
  // {
  //   fmt::println("{}", line);
  // }
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
  _pwd = std::move(_pwd);
  return true;
}

}  // namespace cxxqa
