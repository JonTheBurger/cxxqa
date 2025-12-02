/* Scratch space for local testing */

/* Includes
 ******************************************************************************/
// std
#include <iostream>

// 3rd
#include <uvw.hpp>
#include <uvw/emitter.h>
#include <uvw/loop.h>
#include <uvw/pipe.h>
#include <uvw/process.h>
#include <uvw/stream.h>

// local

/* Types
 ******************************************************************************/

/* Functions
 ******************************************************************************/
auto main() -> int
{
  auto loop = uvw::loop::get_default();
  uvw::process_handle::disable_stdio_inheritance();

  auto proc = loop->resource<uvw::process_handle>();
  proc->flags(
    uvw::process_handle::process_flags::WINDOWS_HIDE |
    uvw::process_handle::process_flags::WINDOWS_HIDE_CONSOLE |
    uvw::process_handle::process_flags::WINDOWS_HIDE_GUI
  );

  auto out = loop->resource<uvw::pipe_handle>();
  out->on<uvw::data_event>([](const uvw::data_event& event, uvw::pipe_handle&) -> void {
    std::cout.write(event.data.get(), event.length);
    std::cout.flush();
  });

  out->on<uvw::end_event>([](const uvw::end_event&, uvw::pipe_handle& handle) -> void {
    std::cout << "\n[Stream ended]" << '\n';
    handle.close();
  });

  proc->on<uvw::exit_event>([](const uvw::exit_event& event, uvw::process_handle& handle) -> void {
    std::cout << "Process exited with code: " << event.status << '\n';
    handle.close();
  });

  proc->on<uvw::error_event>([](const uvw::error_event& event, uvw::process_handle&) -> void {
    std::cerr << "Process error: " << event.what() << '\n';
  });

  char* args[] = {
    const_cast<char*>("tree"),
    const_cast<char*>("/home/jon"),
    nullptr
  };

  proc->stdio({}, uvw::process_handle::stdio_flags::IGNORE_STREAM);
  proc->stdio(
    *out,
    uvw::process_handle::stdio_flags::CREATE_PIPE |
      uvw::process_handle::stdio_flags::WRITABLE_PIPE |
      uvw::process_handle::stdio_flags::READABLE_PIPE
  );

  proc->spawn(args[0], args);

  out->read();

  loop->run();

  return 0;
}
