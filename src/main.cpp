/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std
#include <exception>

// 3rd
#include <cxxqa/core/app.hpp>
#include <cxxqa/parse/cppcheck.hpp>
#include <cxxqa/util/result.hpp>

// local

/* Functions
 ******************************************************************************/
auto main(int argc, char** argv) -> int
{
  try
  {
    return cxxqa::App{ argc, argv }.exec();
  }
  catch (const std::exception& ex)
  {
    std::puts(ex.what());
  }

  return 0;
}
