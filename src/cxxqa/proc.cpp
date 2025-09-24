#include <filesystem>
#include <memory>
#include <string_view>

#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/readable_pipe.hpp>
#include <boost/process/v2/environment.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/v2/stdio.hpp>
#include <boost/system/detail/error_code.hpp>
#include <spdlog/spdlog.h>

#include <cxxqa/exception.hpp>
#include <cxxqa/proc.hpp>

namespace cxxqa {

namespace asio = boost::asio;
namespace fs   = std::filesystem;
namespace proc = boost::process::v2;
namespace sys  = boost::system;

static constexpr char SEP =
#if _WIN32
  '\\';
#else
  '/';
#endif

static constexpr char PATH_SEP =
#if _WIN32
  ';';
#else
  ':';
#endif

struct OutPipe {
  explicit OutPipe(asio::io_context& ctx)
    : pipe{ ctx }
  {
    buffer.reserve(4096);
  }

  asio::readable_pipe pipe;                 // NOLINT(misc-non-private-member-variables-in-classes)
  Process::on_output  callback{ nullptr };  // NOLINT(misc-non-private-member-variables-in-classes)
  std::string         buffer;               // NOLINT(misc-non-private-member-variables-in-classes)
  void*               context{ nullptr };   // NOLINT(misc-non-private-member-variables-in-classes)
};

struct Process::Impl {
  asio::io_context                        ctx;             // NOLINT(misc-non-private-member-variables-in-classes)
  OutPipe                                 std_out{ ctx };  // NOLINT(misc-non-private-member-variables-in-classes)
  OutPipe                                 std_err{ ctx };  // NOLINT(misc-non-private-member-variables-in-classes)
  std::string                             exe;             // NOLINT(misc-non-private-member-variables-in-classes)
  std::vector<std::string>                args;            // NOLINT(misc-non-private-member-variables-in-classes)
  std::flat_map<std::string, std::string> env{};           // NOLINT(misc-non-private-member-variables-in-classes)
  std::string                             pwd;             // NOLINT(misc-non-private-member-variables-in-classes)

  auto async_read(OutPipe& out) -> void
  {
    asio::async_read_until(
      out.pipe,
      asio::dynamic_buffer(out.buffer),
      '\n',
      [this, &out](const sys::error_code& err, std::size_t) {
        if (err || err == asio::error::eof || err == asio::error::broken_pipe)
        {
          return;
        }

        auto end  = out.buffer.rfind('\n');
        auto view = std::string_view{ out.buffer.c_str(), end };
        out.callback(out.context, view);

        out.buffer = out.buffer.substr(end + sizeof('\n'));
        this->async_read(out);
      }
    );
  }
};

Process::Process(std::string_view executable)
  : _self{ std::make_unique<Process::Impl>() }
{
  _self->exe = proc::environment::find_executable(executable);
}

Process::~Process() = default;

auto Process::with_args(std::vector<std::string> arguments) -> Process&
{
  _self->args = std::move(arguments);
  return *this;
}

auto Process::with_env(std::flat_map<std::string, std::string> environment) -> Process&
{
  _self->env = std::move(environment);
  return *this;
}

auto Process::with_pwd(std::string directory) -> Process&
{
  _self->pwd = std::move(directory);
  return *this;
}

auto Process::on_stdout(on_output callback, void* context) -> void
{
  _self->std_out.callback = callback;
  _self->std_out.context  = context;
}

auto Process::on_stderr(on_output callback, void* context) -> void
{
  _self->std_err.callback = callback;
  _self->std_err.context  = context;
}

auto Process::execute() -> int32_t
{
  proc::process child(
    _self->ctx,
    _self->exe,
    _self->args,
    proc::process_stdio{
      .in  = {},
      .out = _self->std_out.pipe,
      .err = _self->std_err.pipe,
    },
    proc::process_environment(_self->env)
  );

  _self->async_read(_self->std_out);
  _self->async_read(_self->std_err);
  _self->ctx.run();

  fmt::println("Done");

  return child.exit_code();
}
}  // namespace cxxqa
