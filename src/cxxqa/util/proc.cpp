/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std
#include <memory>
#include <ranges>
#include <string_view>
#include <vector>

// 3rd
#include <au/au.hh>
#include <au/quantity.hh>
#include <au/units/bytes.hh>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/readable_pipe.hpp>
#include <boost/process/v2/environment.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/v2/start_dir.hpp>
#include <boost/process/v2/stdio.hpp>
#include <boost/system/detail/error_code.hpp>
#include <spdlog/spdlog.h>

// local
#include <cxxqa/util/proc.hpp>

// namespace
namespace cxxqa {
namespace asio = boost::asio;
namespace log  = spdlog;
namespace proc = boost::process::v2;
namespace sys  = boost::system;
namespace unit = au;

/* Functions
 ******************************************************************************/
namespace {
void collect_lines(void* ptr, std::string_view chunk)
{
  auto* lines = static_cast<std::vector<std::string>*>(ptr);
  for (const auto& range : std::views::split(chunk, '\n'))
  {
    lines->emplace_back(range.begin(), range.end());
  }
}

void collect_text(void* ptr, std::string_view chunk)
{
  auto* text = static_cast<std::string*>(ptr);
  text->append(chunk);
}

}  // namespace

/* Constants
 ******************************************************************************/
using KiB = unit::Kibi<unit::Bytes>;

/* Types
 ******************************************************************************/
struct OutPipe {
  explicit OutPipe(asio::io_context& ctx)
    : pipe{ ctx }
  {
    auto cap = unit::make_quantity<KiB, uint32_t>(4);
    buffer.reserve(cap.in(unit::bytes));
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
          log::trace("EOF");
          return;
        }

        auto end  = out.buffer.rfind('\n');
        auto view = std::string_view{ out.buffer.c_str(), end };
        log::trace("{}", view);
        out.callback(out.context, view);

        out.buffer = out.buffer.substr(end + sizeof('\n'));
        this->async_read(out);
      }
    );
  }
};

/* Functions
 ******************************************************************************/
Process::Process(std::string_view executable)
  : _self{ std::make_unique<Process::Impl>() }
{
  _self->exe = proc::environment::find_executable(executable).string();
  _self->pwd = std::filesystem::current_path().string();
}

Process::Process(std::string_view executable, std::vector<std::string> arguments)
  : Process(executable)
{
  this->with_args(std::move(arguments));
}

Process::~Process() = default;

auto Process::exe() const noexcept -> std::string_view
{
  return _self->exe;
}

auto Process::args() const noexcept -> const std::vector<std::string>&
{
  return _self->args;
}

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

auto Process::run() -> int32_t
{
  log::debug("{} {}", _self->exe, _self->args | std::views::join_with(' ') | std::ranges::to<std::string>());
  proc::process child(
    _self->ctx,
    _self->exe,
    _self->args,
    proc::process_stdio{
      .in  = {},
      .out = _self->std_out.pipe,
      .err = _self->std_err.pipe,
    },
    proc::process_start_dir(_self->pwd),
    proc::process_environment(_self->env)
  );

  _self->async_read(_self->std_out);
  _self->async_read(_self->std_err);
  _self->ctx.run();
  child.wait();

  log::trace("exit: {}", child.exit_code());
  return child.exit_code();
}

auto Process::run_lines() -> Lines
{
  Lines lines;
  on_stdout(collect_lines, &lines.stdout);
  on_stderr(collect_lines, &lines.stderr);
  lines.exit_code = run();
  return lines;
}

auto Process::run_text() -> Text
{
  Text text;
  on_stdout(collect_text, &text.stdout);
  on_stderr(collect_text, &text.stderr);
  text.exit_code = run();
  return text;
}

}  // namespace cxxqa

auto fmt::formatter<cxxqa::Process>::format(const cxxqa::Process& self, format_context& ctx) const -> format_context::iterator
{
  auto out = fmt::format_to(ctx.out(), "\"{}\"", self.exe());
  for (const auto& arg : self.args())
  {
    out = fmt::format_to(out, " \"{}\"", arg);
  }
  return out;
}
