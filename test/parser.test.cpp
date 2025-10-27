#include <catch2/catch_test_macros.hpp>

#include <cxxqa/parse/parser.hpp>

SCENARIO("to_lower", "[parser]")  // NOLINT
{
  SECTION("Uppercase letters are lowered")
  {
    REQUIRE('a' == cxxqa::to_lower('A'));
    REQUIRE('s' == cxxqa::to_lower('S'));
    REQUIRE('z' == cxxqa::to_lower('Z'));
  }

  SECTION("Lowercase letters are untouched")
  {
    REQUIRE('a' == cxxqa::to_lower('a'));
    REQUIRE('s' == cxxqa::to_lower('s'));
    REQUIRE('z' == cxxqa::to_lower('z'));
  }

  SECTION("Non-letters are untouched")
  {
    REQUIRE('@' == cxxqa::to_lower('@'));
    REQUIRE('[' == cxxqa::to_lower('['));
    REQUIRE('`' == cxxqa::to_lower('`'));
    REQUIRE('{' == cxxqa::to_lower('{'));
  }
}

SCENARIO("to_upper", "[parser]")  // NOLINT
{
  SECTION("Uppercase letters are untouched")
  {
    REQUIRE('A' == cxxqa::to_upper('A'));
    REQUIRE('S' == cxxqa::to_upper('S'));
    REQUIRE('Z' == cxxqa::to_upper('Z'));
  }

  SECTION("Lowercase letters are uppered")
  {
    REQUIRE('A' == cxxqa::to_upper('a'));
    REQUIRE('S' == cxxqa::to_upper('s'));
    REQUIRE('Z' == cxxqa::to_upper('z'));
  }

  SECTION("Non-letters are untouched")
  {
    REQUIRE('@' == cxxqa::to_upper('@'));
    REQUIRE('[' == cxxqa::to_upper('['));
    REQUIRE('`' == cxxqa::to_upper('`'));
    REQUIRE('{' == cxxqa::to_upper('{'));
  }
}

SCENARIO("parse.until...", "[until]")  // NOLINT
{
  auto parse = cxxqa::Parser("foo/bar.h");

  GIVEN(parse.string())
  {
    WHEN("parsing until '/'")
    {
      auto str = parse.until("/").consume_str();

      THEN("the consumed string is 'foo'")
      {
        REQUIRE("foo" == str);
      }

      AND_THEN("the remaining string is '/bar.h'")
      {
        REQUIRE("/bar.h" == parse.string());
      }
    }

    WHEN("parsing until_and_past '/'")
    {
      auto str = parse.until_and_past("/").consume_str();

      THEN("the consumed string is 'foo'")
      {
        REQUIRE("foo" == str);
      }

      AND_THEN("the remaining string is 'bar.h'")
      {
        REQUIRE("bar.h" == parse.string());
      }
    }

    WHEN("parsing until_and_including '/'")
    {
      auto str = parse.until_and_including("/").consume_str();

      THEN("the consumed string is 'foo/'")
      {
        REQUIRE("foo/" == str);
      }

      AND_THEN("the remaining string is 'bar.h'")
      {
        REQUIRE("bar.h" == parse.string());
      }
    }
  }
}

SCENARIO("parse.skip...", "[skip]")  // NOLINT
{
  auto parse = cxxqa::Parser("foo/bar.h");

  GIVEN(parse.string())
  {
    WHEN("skip 5")
    {
      parse.skip(5);
      THEN("the remaining string is 'ar.h'")
      {
        REQUIRE("ar.h" == parse.string());
      }
    }

    WHEN("skip_while(not '/')")
    {
      parse.skip_while(*+[](char chr) { return chr != '/'; });
      THEN("the remaining string is '/bar.h'")
      {
        REQUIRE("/bar.h" == parse.string());
      }
    }
  }
}

SCENARIO("parse.to_newline", "[to_newline]") // NOLINT
{
  auto parse = cxxqa::Parser("Hello\r\nWorld\nAnd\rGoodbye");

  auto str = parse.to_newline().consume_str();
  REQUIRE("Hello" == str);
}

SCENARIO("Parser", "[example]")  // NOLINT
{
  auto parse = cxxqa::Parser("test-labelled-ranges.c:9:6: error: mismatching types: 'int' and 'const char *'");

  auto str = parse.until_and_past(":").and_rtrim().consume_str();
  REQUIRE("test-labelled-ranges.c" == str);
  REQUIRE(parse.string() == "9:6: error: mismatching types: 'int' and 'const char *'");

  auto num = parse.until_and_past(":").and_rtrim().consume_uint();
  REQUIRE(9 == num);
  REQUIRE(parse.string() == "6: error: mismatching types: 'int' and 'const char *'");

  num = parse.until_and_past(":").and_rtrim().consume_uint();
  REQUIRE(6 == num);
  REQUIRE(parse.string() == "error: mismatching types: 'int' and 'const char *'");

  str = parse.until_and_past(":").and_rtrim().consume_str();
  REQUIRE("error" == str);
  REQUIRE(parse.string() == "mismatching types: 'int' and 'const char *'");

  str = parse.to_eof().consume_str();
  REQUIRE("mismatching types: 'int' and 'const char *'" == str);
  REQUIRE(parse.string() == "");
}

SCENARIO("Parser2", "[example2]")
{
  std::string_view message = R"(/home/jon/Projects/cxx/cxxqa/src/cxxqa/parse/diagnostic.cpp:10:8: warning: variable ‘parse’ set but not used [-Wunused-but-set-variable]
   10 |   auto parse = Parser(str);
      |        ^~~~~
)";
}
