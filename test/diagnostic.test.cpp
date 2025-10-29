#include <catch2/catch_test_macros.hpp>

#include <cxxqa/parse/diagnostic.hpp>

SCENARIO("Multiple Diagnostics")  // NOLINT
{
  std::string_view message = R"(C:/Users/vagrant/My Documents/diagnostic.cpp:10:8: warning: variable ‘parse’ set but not used [-Wunused-but-set-variable]
   10 |   auto parse = Parser(str);
      |        ^~~~~
C:/Users/vagrant/My Documents/serif.cpp:324:8: error: Missing attribute [[nodiscard]] [-Wmissing-attribute]
  324 |   auto empty() -> bool;
      |   ^~~~~
)";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "C:/Users/vagrant/My Documents/diagnostic.cpp");
  REQUIRE(diagnostic->line == 10);
  REQUIRE(diagnostic->column == 8);
  REQUIRE(diagnostic->severity == "warning");
  REQUIRE(diagnostic->message == "variable ‘parse’ set but not used");
  REQUIRE(diagnostic->category == "-Wunused-but-set-variable");
  REQUIRE(diagnostic->source == R"(   10 |   auto parse = Parser(str);
      |        ^~~~~
)");
}

SCENARIO("Full Diagnostic")  // NOLINT
{
  std::string_view message = R"(C:/Users/vagrant/My Documents/diagnostic.cpp:10:8: warning: variable ‘parse’ set but not used [-Wunused-but-set-variable]
   10 |   auto parse = Parser(str);
      |        ^~~~~
)";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "C:/Users/vagrant/My Documents/diagnostic.cpp");
  REQUIRE(diagnostic->line == 10);
  REQUIRE(diagnostic->column == 8);
  REQUIRE(diagnostic->severity == "warning");
  REQUIRE(diagnostic->message == "variable ‘parse’ set but not used");
  REQUIRE(diagnostic->category == "-Wunused-but-set-variable");
  REQUIRE(diagnostic->source == R"(   10 |   auto parse = Parser(str);
      |        ^~~~~
)");
}

SCENARIO("Minimal Diagnostic")  // NOLINT
{
  std::string_view message = "/home/vagrant/foo.cxx: something went wrong";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "/home/vagrant/foo.cxx");
  REQUIRE(diagnostic->message == "something went wrong");
}

SCENARIO("Diagnostic with line")  // NOLINT
{
  std::string_view message = R"(\\net\vagrant\foo.c:375: something went wrong)";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "\\\\net\\vagrant\\foo.c");
  REQUIRE(diagnostic->line == 375);
  REQUIRE(diagnostic->message == "something went wrong");
}

SCENARIO("Diagnostic with column")  // NOLINT
{
  std::string_view message = R"(\\net\vagrant\foo.c:375:12: something went wrong)";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "\\\\net\\vagrant\\foo.c");
  REQUIRE(diagnostic->line == 375);
  REQUIRE(diagnostic->column == 12);
  REQUIRE(diagnostic->message == "something went wrong");
}

SCENARIO("Diagnostic with severity")  // NOLINT
{
  std::string_view message = R"(\\net\vagrant\foo.c:375: warning: something went wrong)";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "\\\\net\\vagrant\\foo.c");
  REQUIRE(diagnostic->line == 375);
  REQUIRE(diagnostic->severity == "warning");
  REQUIRE(diagnostic->message == "something went wrong");
}

SCENARIO("Diagnostic with severity only")  // NOLINT
{
  std::string_view message = R"(\\net\vagrant\foo.c: warning: something went wrong)";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "\\\\net\\vagrant\\foo.c");
  REQUIRE(diagnostic->severity == "warning");
  REQUIRE(diagnostic->message == "something went wrong");
}

SCENARIO("Diagnostic spaces in path")  // NOLINT
{
  std::string_view message = R"(C:\Program Files\foo.c: fatal error: something went wrong [-Wsomething])";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "C:\\Program Files\\foo.c");
  REQUIRE(diagnostic->severity == "fatal error");
  REQUIRE(diagnostic->message == "something went wrong");
  REQUIRE(diagnostic->category == "-Wsomething");
}

SCENARIO("Diagnostic with brackets")  // NOLINT
{
  std::string_view message = R"(C:\Program Files\foo.c: error: operator[] should have [[nodiscard]] [-Wsomething])";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "C:\\Program Files\\foo.c");
  REQUIRE(diagnostic->severity == "error");
  REQUIRE(diagnostic->message == "operator[] should have [[nodiscard]]");
  REQUIRE(diagnostic->category == "-Wsomething");
}
