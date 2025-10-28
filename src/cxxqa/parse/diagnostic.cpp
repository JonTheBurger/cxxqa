#include <fmt/format.h>

#include <cxxqa/parse/diagnostic.hpp>
#include <cxxqa/parse/parser.hpp>

namespace cxxqa {
auto Diagnostic::from_string(std::string_view str) -> std::optional<Diagnostic>
{
  using namespace std::string_view_literals;
  auto result = std::optional<Diagnostic>{ std::nullopt };
  auto parse  = Parser(str);

  Diagnostic diagnostic{};

  if (auto file = parse.until_and_past(":").and_rtrim().consume_str(); file)
  {
    diagnostic.file = *file;
  }
  else
  {
    return result;
  }

  // Avoid mistaking Windows drive letter for the full file path before the colon:
  if (parse.pos() == "C:"sv.length())
  {
    if (auto file = parse.until_and_past(":").and_rtrim().consume_str(); file)
    {
      diagnostic.file += ':';
      diagnostic.file += *file;
    }
    else
    {
      return result;
    }
  }

  if (auto line = parse.until_and_past(":").and_rtrim().consume_uint(); line)
  {
    diagnostic.line = *line;

    if (auto col = parse.until_and_past(":").and_rtrim().consume_uint(); col)
    {
      diagnostic.column = *col;
    }
  }

  if (auto severity = parse.until_and_past(":").or_backtrack().and_rtrim().consume_str(); severity)
  {
    if ((*severity == "warning") || (*severity == "error") || (*severity == "note") || (*severity == "fatal error") || (*severity == "internal compiler error") || (*severity == "sorry, unimplemented"))
    {
      diagnostic.severity = *severity;
    }
  }

  if (auto message = parse.to_newline().to_eof().consume_str(); message)
  {
    diagnostic.message = *message;
    if (diagnostic.message.ends_with(']'))
    {
      // variable ‘parse’ set but not used [-Wunused-but-set-variable]
      //                               pos ^
      auto pos = diagnostic.message.rfind('[');

      if (pos != std::string::npos)
      {
        // variable ‘parse’ set but not used [-Wunused-but-set-variable]
        //                             substr ^~~~~~~~~~~~~~~~~~~~~~~~^
        diagnostic.category = diagnostic.message.substr(pos + 1, diagnostic.message.size() - pos - 2);

        // variable ‘parse’ set but not used [-Wunused-but-set-variable]
        // ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^ substr
        diagnostic.message  = diagnostic.message.substr(0, pos - 1);
      }
    }
  }
  else
  {
    return result;
  }

  if (auto source = parse.to_eof().consume_str(); source)
  {
    diagnostic.source = *source;
  }

  result = diagnostic;
  return result;
}
}  // namespace cxxqa

auto fmt::formatter<cxxqa::Diagnostic>::format(const cxxqa::Diagnostic& self, format_context& ctx) const -> format_context::iterator
{
  return formatter<std::string_view>::format("", ctx);
}
