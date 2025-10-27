#include <catch2/catch_test_macros.hpp>

#include <cxxqa/parse/diagnostic.hpp>

SCENARIO("HelloLexy", "[example]")
{
  GIVEN("Foo")
  {
    WHEN("Bar")
    {
      THEN("Baz")
      {
        REQUIRE(0 == 0);
      }
    }
  }
}

SCENARIO("Parser 2", "[example]")
{
  std::string_view message = R"(/home/jon/Projects/cxx/cxxqa/src/cxxqa/parse/diagnostic.cpp:10:8: warning: variable ‘parse’ set but not used [-Wunused-but-set-variable]
   10 |   auto parse = Parser(str);
      |        ^~~~~
)";

}
