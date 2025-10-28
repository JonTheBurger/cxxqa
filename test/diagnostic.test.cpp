#include <catch2/catch_test_macros.hpp>

#include <cxxqa/parse/diagnostic.hpp>

SCENARIO("Full Diagnostic")
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

SCENARIO("Minimal Diagnostic")
{
  std::string_view message = "/home/vagrant/foo.cxx: you smell";

  auto diagnostic = cxxqa::Diagnostic::from_string(message);
  REQUIRE(diagnostic.has_value());

  REQUIRE(diagnostic->file == "/home/vagrant/foo.cxx");
  REQUIRE(diagnostic->message == "you smell");
}
