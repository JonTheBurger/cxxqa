cmake_minimum_required(VERSION 3.28...4.1)
project(cxxqa)

find_package(Au REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio filesystem process)
find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)
find_package(re2 REQUIRED)
find_package(spdlog REQUIRED)

add_executable(cxxqa)
target_sources(cxxqa
  PRIVATE
    src/cxxqa/core/app.cpp
    src/cxxqa/core/app.hpp
    src/cxxqa/core/config.cpp
    src/cxxqa/core/config.hpp
    src/cxxqa/core/exception.cpp
    src/cxxqa/core/exception.hpp
    src/cxxqa/parse/compile_command.cpp
    src/cxxqa/parse/compile_command.hpp
    src/cxxqa/tool/git.cpp
    src/cxxqa/util/proc.cpp
    src/main.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src
    FILES
)
target_link_libraries(cxxqa
  PRIVATE
    Au::au
    Boost::asio
    Boost::process
    CLI11::CLI11
    fmt::fmt
    re2::re2
    spdlog::spdlog
)
target_compile_features(cxxqa PRIVATE cxx_std_23)

install(TARGETS cxxqa)
