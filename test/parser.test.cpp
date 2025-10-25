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

SCENARIO("Parser", "[example]")  // NOLINT
{
  auto parse = cxxqa::Parser(
    "test-labelled-ranges.c:9:6: error: mismatching types: 'int' and 'const char *'"
  );

  auto str = parse.until_passing(":").and_rtrim().consume_str();
  REQUIRE("test-labelled-ranges.c" == str);
  REQUIRE(parse.string() == "9:6: error: mismatching types: 'int' and 'const char *'");

  auto num = parse.until_passing(":").and_rtrim().consume_int();
  REQUIRE(9 == num);
  REQUIRE(parse.string() == "6: error: mismatching types: 'int' and 'const char *'");

  num = parse.until_passing(":").and_rtrim().consume_int();
  REQUIRE(6 == num);
  REQUIRE(parse.string() == "error: mismatching types: 'int' and 'const char *'");

  str = parse.until_passing(":").and_rtrim().consume_str();
  REQUIRE("error" == str);
  REQUIRE(parse.string() == "mismatching types: 'int' and 'const char *'");

  str = parse.to_eof().consume_str();
  REQUIRE("mismatching types: 'int' and 'const char *'" == str);
  REQUIRE(parse.string() == "");
}
