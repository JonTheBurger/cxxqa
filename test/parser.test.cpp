#include <catch2/catch_test_macros.hpp>

#include <cxxqa/parse/parser.hpp>

SCENARIO("Parser", "[example]")
{
  auto parse = cxxqa::Parser(
    "test-labelled-ranges.c:9:6: error: mismatching types: 'int' and 'const char *'"
  );

  auto str = parse.until_passing(":").consume_str();
  REQUIRE("test-labelled-ranges.c" == str);

  REQUIRE(parse.string() == "9:6: error: mismatching types: 'int' and 'const char *'");

  // GIVEN("Foo")
  // {
  //   WHEN("Bar")
  //   {
  //     THEN("Baz")
  //     {
  //       REQUIRE(0 == 0);
  //     }
  //   }
  // }
}

