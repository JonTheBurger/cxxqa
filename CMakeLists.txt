cmake_minimum_required(VERSION 3.28...4.1)
project(sharif)
include(CTest)

find_package(Au REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio filesystem process)
find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)
find_package(re2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(pugixml REQUIRED)
find_package(uvw REQUIRED)

add_library(sharif.core)
target_sources(sharif.core
  PRIVATE
    src/sharif/core/app.cpp
    src/sharif/core/config.cpp
    src/sharif/parse/compile_command.cpp
    src/sharif/parse/cppcheck.cpp
    src/sharif/parse/diagnostic.cpp
    src/sharif/parse/parser.cpp
    src/sharif/parse/sarif.cpp
    src/sharif/tool/git.cpp
    src/sharif/util/proc.cpp
    src/sharif/util/result.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      src
    FILES
      src/sharif/core/app.hpp
      src/sharif/core/config.hpp
      src/sharif/parse/compile_command.hpp
      src/sharif/parse/detail/sarif_spec.hpp
      src/sharif/parse/diagnostic.hpp
      src/sharif/parse/parser.hpp
      src/sharif/parse/sarif.hpp
      src/sharif/tool/git.hpp
      src/sharif/util/proc.hpp
      src/sharif/util/result.hpp
)
target_link_libraries(sharif.core
  PUBLIC
    Au::au
    Boost::asio
    Boost::process
    CLI11::CLI11
    fmt::fmt
    pugixml::pugixml
    re2::re2
    spdlog::spdlog
)
target_compile_features(sharif.core PUBLIC cxx_std_23)

add_executable(sharif src/main.cpp)
target_link_libraries(sharif PRIVATE sharif.core)
install(TARGETS sharif sharif.core)

add_executable(scratch src/scratch.cpp)
target_link_libraries(scratch PRIVATE sharif.core uvw::uvw)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
