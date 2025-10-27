cmake_minimum_required(VERSION 3.28...4.1)
project(cxxqa)
include(CTest)

find_package(Au REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio filesystem process)
find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)
find_package(re2 REQUIRED)
find_package(spdlog REQUIRED)

add_library(cxxqa.core)
target_sources(cxxqa.core
  PRIVATE
    src/cxxqa/core/app.cpp
    src/cxxqa/core/config.cpp
    src/cxxqa/core/exception.cpp
    src/cxxqa/parse/compile_command.cpp
    src/cxxqa/parse/diagnostic.cpp
    src/cxxqa/parse/parser.cpp
    src/cxxqa/tool/git.cpp
    src/cxxqa/util/proc.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src
    FILES
      src/cxxqa/core/app.hpp
      src/cxxqa/core/config.hpp
      src/cxxqa/core/exception.hpp
      src/cxxqa/parse/compile_command.hpp
      src/cxxqa/parse/diagnostic.hpp
      src/cxxqa/parse/parser.hpp
      src/cxxqa/tool/git.hpp
      src/cxxqa/util/proc.hpp
)
target_link_libraries(cxxqa.core
  PUBLIC
    Au::au
    Boost::asio
    Boost::process
    CLI11::CLI11
    fmt::fmt
    re2::re2
    spdlog::spdlog
)
target_compile_features(cxxqa.core PUBLIC cxx_std_23)

add_executable(cxxqa src/main.cpp)
target_link_libraries(cxxqa PRIVATE cxxqa.core)
install(TARGETS cxxqa cxxqa.core)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
