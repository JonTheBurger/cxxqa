include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)
include(Catch)
link_libraries(cxxqa.core Catch2::Catch2WithMain)

add_executable(diagnostic.test diagnostic.test.cpp)
catch_discover_tests(diagnostic.test)

add_executable(parser.test parser.test.cpp)
catch_discover_tests(parser.test)

# add_test(NAME diagnostic.test COMMAND diagnostic.test)

# get_property(all_TESTS DIRECTORY . PROPERTY BUILDSYSTEM_TARGETS)
# foreach(test_TARGET IN LISTS all_TESTS)
#   catch_discover_tests(${test_TARGET})
# endforeach()
