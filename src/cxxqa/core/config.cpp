#include <cstdlib>
#include <filesystem>
#include <optional>
#include <ranges>
#include <string_view>

#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Option.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <re2/re2.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <cxxqa/core/config.hpp>

namespace {
namespace fs  = std::filesystem;
namespace log = spdlog;

// TODO: configure logger

constexpr char PATH_ENV_SEP =
#if defined(_WIN32) && !defined(__CYGWIN__)
  ';'
#else
  ':'
#endif
  ;

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

auto environment(const char* name) -> std::optional<const char*>
{
  static std::mutex mtx;
  std::lock_guard   lock{ mtx };

  auto* var = std::getenv(name);  // NOLINT(concurrency-mt-unsafe) static lock_guarded
  log::trace("Looking up $ENV{{{}}}", name);
  return (var != nullptr) ? (std::optional{ var }) : std::nullopt;
}

/** @returns a list of config file directory candidates based on common environment
 * variables and default locations.
 *
 * @param app Name of the application's sub-directory.
 */
auto get_config_dirs(std::string_view app = "cxxqa") -> std::vector<fs::path>
{
  std::vector<fs::path> directories{ "." };
  log::trace("Searching for config dirs...");

  if (auto home = environment("XDG_CONFIG_HOME"); home)
  {
    // $XDG_CONFIG_HOME/<app>
    directories.emplace_back(*home);
    directories.back() /= app;
  }

  if (auto home = environment("HOME"); home)
  {
    // ~/.config/<app>
    directories.emplace_back(*home);
    directories.back() /= ".config";
    directories.back() /= app;

    // ~/Library/Application Support/<app>
    directories.emplace_back(*home);
    directories.back() /= "Library/Application Support";
    directories.back() /= app;
  }

  if (auto appdata = environment("AppData"); appdata)
  {
    // ~/AppData/Roaming/<app>
    directories.emplace_back(*appdata);
    directories.back() /= app;
  }

  if (auto cdirs = environment("XDG_CONFIG_DIRS"); cdirs)
  {
    // $XDG_CONFIG_DIRS/<app>
    for (const auto cfgdir : std::views::split(std::string_view{ *cdirs }, PATH_ENV_SEP))
    {
      directories.emplace_back(std::string_view{ cfgdir });
      directories.back() /= app;
    }
  }

  // /etc/<app>
  directories.emplace_back("/etc");
  directories.back() /= app;

  return directories;
}

auto config_dir_validator(std::string_view app = "cxxqa") -> CLI::Validator
{
  CLI::Validator validator = CLI::FileOnDefaultPath(".", false);
  for (auto&& dir : get_config_dirs(app))
  {
    validator = validator | CLI::FileOnDefaultPath(std::move(dir).string(), false);
  }
  return validator;
}

}  // namespace

namespace cxxqa {
Config::Config() = default;

auto Config::from_cli(int argc, char** argv) -> Config
{
  Config self;

  CLI::App cli{ "cxxqa" };
  cli.description("C/C++ project quality assurance driver.");
  cli.usage(fmt::format("{} {}", fmt::styled("USAGE:", fmt::emphasis::bold | fmt::fg(fmt::color::lime_green)), fmt::styled("cxxqa [OPTIONS] <SUBCOMMAND>", fmt::fg(fmt::color::cyan))));
  cli.formatter(std::make_shared<ColorFormatter>());
  cli.allow_config_extras(true);
  cli.set_help_all_flag("--help-all", "Show options for all sub-commands");
  cli.set_version_flag("-V,--version", "0.1.0");
  cli.set_config("-c,--config", "cxxqa.toml")->transform(config_dir_validator("cxxqa"));
  cli.add_flag("-v,--verbose", self._verbosity, "Verbosity; can be specified multiple times (-vvv)")->envname("VERBOSE");
  cli.add_option("include", self._include, "Glob expressions or files to include")->default_val(std::vector<std::string>{ "*.cpp", "*.hpp" });
  // TODO: maybe this shouldn't be a vector
  // TODO: specify if we normalize paths to '/', and if this matches on relative or absolute paths
  cli.add_option("-x,--exclude", self._exclude, "Regular expressions of files to ignore");
  cli.add_option("-o,--output", self._output, "Output path");
  cli.add_option("-l,--log", self._log_file, "Log file output path");
  // cli.add_option("-f,--format", self._format, "Input/output format");
  cli.add_option("-p,--project", self._project, "Path to compile_commands.json");
  cli.add_option("--preset", self._preset, "CMakePresets.json configuration preset used to lookup 'compile_commands.json'");

  CLI::App* lint = cli.add_subcommand("lint");

  CLI::App* inspect = cli.add_subcommand("inspect");
  inspect->description("Inspect the cxxqa application for troubleshooting");
  inspect->add_flag("--config-values", self._troubleshoot.config_values, "Show resolved configuration");
  inspect->add_flag("--config-file", self._troubleshoot.config_file, "Show config file directory candidates");
  inspect->add_flag("--files", self._troubleshoot.files, "Show files included and excluded from project");

  try
  {
    cli.parse(argc, argv);

    // Setup Logging
    if (self._verbosity >= 3)
    {
      log::set_level(log::level::trace);
    }
    else if (self._verbosity >= 2)
    {
      log::set_level(log::level::debug);
    }
    else if (self._verbosity >= 1)
    {
      log::set_level(log::level::info);
    }

    // Check for extra arguments
    auto extra = cli.remaining(true);
    if (extra.size() > 0)
    {
      log::warn("Found unrecognized arguments: {}", extra);
    }

    // Handle inspect sub-command
    if (inspect->parsed())
    {
      if (self._troubleshoot.config_values)
      {
        fmt::println("{}", cli.config_to_str(true));
      }
      if (self._troubleshoot.config_file)
      {
        for (const auto& path : get_config_dirs("cxxqa"))
        {
          fmt::println("{}", path.string());
        }
        fmt::println("");
      }
    }
  }
  catch (const CLI::ParseError& e)  // TODO: this should probably propagate up?
  {
    cli.exit(e);
  }
  return self;
}

auto Config::exclude_matches(std::string_view str) const noexcept -> bool
{
  return std::ranges::any_of(
    exclude(), [&str](const std::string& pattern) { return RE2::PartialMatch(str, pattern); }
  );
}

auto Config::include() const noexcept -> const std::vector<std::string>&
{
  return _include;
}

auto Config::exclude() const noexcept -> const std::vector<std::string>&
{
  return _exclude;
}

auto Config::output() const noexcept -> const std::string&
{
  return _output;
}

auto Config::log_file() const noexcept -> const std::string&
{
  return _log_file;
}

auto Config::project() const noexcept -> const std::string&
{
  return _project;
}

auto Config::preset() const noexcept -> const std::string&
{
  return _preset;
}

auto Config::verbosity() const noexcept -> unsigned
{
  return _verbosity;
}

auto Config::troubleshoot() const noexcept -> const Troubleshoot&
{
  return _troubleshoot;
}

}  // namespace cxxqa
