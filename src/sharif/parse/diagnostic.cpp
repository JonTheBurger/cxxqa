/** @file
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/
// std

// 3rd

// local
#include <sharif/parse/diagnostic.hpp>
#include <sharif/parse/parser.hpp>

// namespace
namespace sharif {

/* Functions
 ******************************************************************************/
auto Diagnostic::consume_from_string(std::string_view& str) -> std::optional<Diagnostic>
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

  auto pos = parse.pos();
  if (auto severity = parse.until_and_past(":").and_rtrim().consume_str(); severity)
  {
    if ((*severity == "warning") || (*severity == "error") || (*severity == "note") || (*severity == "fatal error") || (*severity == "internal compiler error") || (*severity == "sorry, unimplemented"))
    {
      diagnostic.severity = *severity;
    }
    else
    {
      parse.set_pos(pos);
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
        diagnostic.message = diagnostic.message.substr(0, pos - 1);
      }
    }
  }
  else
  {
    return result;
  }

  // Parse source as long as the line starts with space
  while (parse.to_newline().to_eof().peek() == ' ')
  {
    if (auto source = parse.consume_str(); source)
    {
      diagnostic.source += *source;
      diagnostic.source += '\n';
    }
    else
    {
      break;
    }
  }

  str    = str.substr(parse.pos());
  result = diagnostic;
  return result;
}

auto Diagnostic::parse_all(std::string_view str) -> std::vector<Diagnostic>
{
  std::vector<Diagnostic> diagnostics;
  while (auto diagnostic = Diagnostic::consume_from_string(str))
  {
    diagnostics.push_back(*diagnostic);
  }
  return diagnostics;
}

}  // namespace sharif

auto fmt::formatter<sharif::Diagnostic>::format(const sharif::Diagnostic& self, format_context& ctx) const -> format_context::iterator
{
  // TODO: implement
  return formatter<std::string_view>::format("", ctx);
}
