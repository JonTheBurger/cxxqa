/* Scratch space for local testing */

/* Includes
 ******************************************************************************/
// std

// 3rd

// local
#include <cxxqa/parse/sarif.hpp>
#include <cxxqa/util/fmt.hpp>
#include <cxxqa/util/json.hpp>
#include <cxxqa/util/result.hpp>
#include <glaze/json/generic.hpp>

/* Types
 ******************************************************************************/
struct Item {
  std::string name;
  std::optional<std::flat_map<std::string, glz::generic>> properties;
};

/* Functions
 ******************************************************************************/
auto main() -> int
{
  using namespace cxxqa;

  Item item;
  item.name = "Alice";
  item.properties.emplace();
  item.properties.value()["foo"] = "bar";

  fmt::println("{}", json::write_json(item).value_or(""));

  return 0;
}
