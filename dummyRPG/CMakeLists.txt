cmake_minimum_required(VERSION 3.10.0)
project(dummy)

# Specify C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

###############################################################################
# Library files

add_library(dummyrpg STATIC
    include/dummyrpg/dummy_types.hpp
    include/dummyrpg/floor.hpp
    include/dummyrpg/layer.hpp
    include/dummyrpg/map.hpp
    include/dummyrpg/game.hpp

    src/floor.cpp
    src/map.cpp
    src/game.cpp
)

target_include_directories(dummyrpg PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Add compilation warnings
if(MSVC)
  target_compile_options(dummyrpg PRIVATE /W4 /w14640)
else()
  target_compile_options(dummyrpg PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif()

###############################################################################
# Test files

add_executable(main_tests
    tests/main_tests.cpp
)
target_link_libraries(main_tests dummyrpg)

# Create the gcov target. Run coverage tests with 'make gcov'
add_custom_target(gcov
    COMMAND mkdir -p coverage
    COMMAND ${CMAKE_MAKE_PROGRAM} test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
add_custom_command(TARGET gcov
    COMMAND echo "=================== GCOV ===================="
    COMMAND gcov -b ${CMAKE_SOURCE_DIR}/*.cpp -o ${OBJECT_DIR}
    COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/coverage"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage
    )
add_dependencies(gcov main_tests)

# Testing
enable_testing()

add_test(output_test ${CMAKE_CURRENT_BINARY_DIR}/RunAdder)
set_tests_properties(output_test PROPERTIES PASS_REGULAR_EXPRESSION "")
