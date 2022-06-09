cmake_minimum_required(VERSION 3.11)
project(gtl LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)


Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        origin/devel)
FetchContent_MakeAvailable(Catch2)

Include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        origin/master)
FetchContent_MakeAvailable(pybind11)

# Add your algorithm sources to the list below (space delimited):
set(SOURCES src/board.cpp src/random_rules.cpp)
# Add your headers to the list below (space delimited):
set(HEADERS src/board.hpp src/random_rules.hpp)
# Add your test files to the list below (space delimited):
set(SOURCES_TEST tests/test_random_rules.cpp tests/test_board.cpp)
# set(SOURCES_MAIN sources/main.cpp)

SET(GCC_WARNINGS_COMPILE_FLAGS "-Wextra -pedantic -Wall -Werror")
SET(GCC_UB_COMPILE_FLAGS    "-fsanitize=undefined")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS_COMPILE_FLAGS}")
# Uncomment the following line to find undefined behavior:
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_UB_COMPILE_FLAGS}")

include_directories(src)

add_executable(test_project ${SOURCES_TEST} ${SOURCES} ${HEADERS})
# add_executable(example ${SOURCES_MAIN} ${SOURCES} ${HEADERS})

# target_include_directories(factorial PRIVATE headers)
target_include_directories(test_project PRIVATE HEADERS)
target_link_libraries(test_project Catch2::Catch2WithMain)

add_custom_target(test
    COMMAND test_project
    DEPENDS test_project
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
