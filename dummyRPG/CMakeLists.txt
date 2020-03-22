cmake_minimum_required(VERSION 3.10.0)
project(dummy)

# Specify C++ version
set(CMAKE_CXX_STANDARD 17)

###############################################################################
# Library files

add_library(dummyrpg STATIC
    include/dummyrpg/character.hpp
    include/dummyrpg/dummy_types.hpp
    include/dummyrpg/floor.hpp
    include/dummyrpg/item.hpp
    include/dummyrpg/layer.hpp
    include/dummyrpg/map.hpp
    include/dummyrpg/game.hpp
    include/dummyrpg/sprite.hpp

    src/floor.cpp
    src/map.cpp
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
