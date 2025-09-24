#include <filesystem>
#include <ranges>
#include <string_view>

#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Option.hpp>
#include <fmt/color.h>
#include <fmt/format.h>

#include <cxxqa/config.hpp>

namespace fs = std::filesystem;

namespace {
class ColorFormatter final : public CLI::Formatter {
  using super = CLI::Formatter;

public:
  auto make_group(std::string group, bool is_positional, std::vector<const CLI::Option*> opts) const -> std::string override
  {
    return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::lime_green), "{}", super::make_group(group, is_positional, opts));
  }

  // auto make_positionals(const CLI::App* app) const -> std::string override;

  auto make_subcommands(const CLI::App* app, CLI::AppFormatMode mode) const -> std::string override
  {
    return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::lime_green), "{}", super::make_subcommands(app, mode));
  }

  auto make_subcommand(const CLI::App* sub) const -> std::string override
  {
    return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::cyan), "{}", super::make_subcommand(sub));
  }

  // auto make_expanded(const CLI::App* sub, CLI::AppFormatMode mode) const -> std::string override;

  // auto make_footer(const CLI::App* app) const -> std::string override;

  // auto make_description(const CLI::App* app) const -> std::string override;

  // auto make_usage(const CLI::App* app, std::string name) const -> std::string override;

  // auto make_help(const CLI::App* opt, std::string name, CLI::AppFormatMode mode) const -> std::string override;

  // auto make_option(const CLI::Option* opt, bool is_positional) const -> std::string override;

  auto make_option_name(const CLI::Option* opt, bool is_positional) const -> std::string override
  {
    return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::cyan), "{}", super::make_option_name(opt, is_positional));
  }

  // auto make_option_opts(const CLI::Option* opt) const -> std::string override;

  // auto make_option_usage(const CLI::Option* opt) const -> std::string override;
};

auto get_config_file_for(std::string_view directory, std::string_view app = "cxxqa", std::string_view cfg_file_name = "cxxqa.toml") -> std::string
{
  if (auto dir = fs::path(directory); dir.is_absolute())
  {
    if (auto cfg_file = dir / app / cfg_file_name; fs::is_regular_file(cfg_file))
    {
      return std::move(cfg_file).string();
    }
  }
  return "";
}

auto get_config_file(std::string_view app = "cxxqa", std::string_view ext = ".toml") -> std::string
{
  const std::string cfg_file_name = fmt::format("{}{}", app, ext);

  if (const char* env_var = std::getenv("XDG_CONFIG_HOME"))
  {
    if (auto cfg_file = get_config_file_for(env_var); cfg_file != "")
    {
      return cfg_file;
    }
  }

  if (const char* env_var = std::getenv("XDG_CONFIG_DIRS"))
  {
    for (const auto dir : std::views::split(std::string_view{ env_var }, ':'))
    {
      if (auto cfg_file = get_config_file_for(std::string_view{ dir }); cfg_file != "")
      {
        return cfg_file;
      }
    }
  }

  if (const char* env_var = std::getenv("AppData"))
  {
    if (auto cfg_file = get_config_file_for(env_var); cfg_file != "")
    {
      return cfg_file;
    }
  }

  if (const char* env_var = std::getenv("HOME"))
  {
    if (auto dir = fs::path(env_var); dir.is_absolute())
    {
      if (auto cfg_file = dir / app / ".config" / cfg_file_name; fs::is_regular_file(cfg_file))
      {
        return std::move(cfg_file).string();
      }
    }
  }

  return "";
}
}  // namespace

namespace cxxqa {
Config::Config() = default;

auto Config::from_cli(int argc, char** argv) -> Config
{
  Config config;

  CLI::App cli{ "cxxqa" };
  cli.description("C/C++ project quality assurance driver.");
  cli.usage(fmt::format("{} {}", fmt::styled("USAGE:", fmt::emphasis::bold | fmt::fg(fmt::color::lime_green)), fmt::styled("cxxqa [OPTIONS] <SUBCOMMAND>", fmt::fg(fmt::color::cyan))));
  cli.formatter(std::make_shared<ColorFormatter>());
  cli.allow_config_extras(true);
  cli.set_version_flag("-V,--version", "0.1.0");
  // TODO: @?
  cli.set_config("-c,--config")->transform(CLI::FileOnDefaultPath("cxxqa.toml") | CLI::FileOnDefaultPath(get_config_file()));
  cli.add_flag("-v,--verbose", config._verbosity, "Verbosity; can be specified multiple times (-vvv)")->envname("VERBOSE");
  cli.add_flag("-l,--log", config._log_file, "Log file output path");
  cli.add_flag("-o,--output", config._output, "Output path");
  cli.add_flag("-x,--exclude", config._excludes, "Regular expressions of files to ignore");
  cli.add_flag("--compile-commands", config._compile_commands_json, "Path to compile_commands.json");
  cli.add_flag("--preset", config._preset, "CMakePresets.json configuration preset used to lookup 'compile_commands.json'");

  CLI::App* lint = cli.add_subcommand("lint");

  try
  {
    cli.parse(argc, argv);
  }
  catch (const CLI::ParseError& e)
  {
    cli.exit(e);
  }
  return config;
}

auto Config::verbosity() const noexcept -> unsigned
{
  return _verbosity;
}

auto Config::log_file() const noexcept -> const std::string&
{
  return _log_file;
}

auto Config::compile_commands_json() const noexcept -> const std::string&
{
  return _compile_commands_json;
}

auto Config::preset() const noexcept -> const std::string&
{
  return _preset;
}

}  // namespace cxxqa
