/** @file
 *
 ******************************************************************************/
#pragma once

/* Includes
 ******************************************************************************/
// std
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// 3rd

// local

// namespace
namespace sharif {

/* Types
 ******************************************************************************/
class App {
public:
  App();
  App(int argc, char** argv);
  explicit App(std::vector<std::string> args);
  ~App();

  auto set_args(std::vector<std::string> args) -> void;
  auto argc() const noexcept -> int;
  auto argv() noexcept -> char**;
  auto exec() -> int;

  auto project_dir() -> const std::filesystem::path&;

private:
  struct Impl;
  std::unique_ptr<Impl> _self;
};

}  // namespace sharif
