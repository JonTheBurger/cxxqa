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
