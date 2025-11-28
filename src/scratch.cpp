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
#include "cxxqa/parse/detail/sarif_spec.hpp"

/* Types
 ******************************************************************************/

/* Functions
 ******************************************************************************/
auto main() -> int
{
  using namespace cxxqa;

  Sarif sarif;
  sarif::Run run{
    .tool{
      .driver{
        .name = "CodeScanner",
      }
    }
  };
  run.results.emplace();
  run.results.value().push_back(sarif::Result{
    .ruleId = "no-unused-vars",
    .level = sarif::Level::error,
    .message = {
      .text = "'x' is assigned a value but never used.",
    },
    // .locations = std::vector<sarif::Location>{
    //   sarif::Location{
    //     .physicalLocation = sarif::PhysicalLocation{
    //       .artifactLocation = sarif::ArtifactLocation{
    //         .uri = "file:///path/to/file.cpp",
    //       },
    //       .region = sarif::Region{
    //         .startLine = 10,
    //         .startColumn = 5,
    //       },
    //     },
    //   },
    // },
  });

  sarif.runs.push_back(std::move(run));

  fmt::println("{}", sarif);

  return 0;
}
