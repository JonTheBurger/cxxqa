cmake_minimum_required(VERSION 3.28...4.1)
project(cxxqa)

find_package(Boost REQUIRED COMPONENTS asio process)
find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)
find_package(re2 REQUIRED)
find_package(spdlog REQUIRED)

add_executable(cxxqa)
target_sources(cxxqa
  PRIVATE
    src/main.cpp
    src/cxxqa/app.cpp
    src/cxxqa/compile_command.cpp
    src/cxxqa/config.cpp
    src/cxxqa/exception.cpp
    src/cxxqa/git.cpp
    src/cxxqa/proc.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src
    FILES
)
target_link_libraries(cxxqa
  PRIVATE
    Boost::asio
    Boost::process
    CLI11::CLI11
    fmt::fmt
    re2::re2
    spdlog::spdlog
)
target_compile_features(cxxqa PRIVATE cxx_std_23)

install(TARGETS cxxqa)
