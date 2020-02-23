cmake_minimum_required(VERSION 3.10.0)
project(dummy)

# Specify C++ version
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(LIB_HEADERS 
    include/dummyrpg/dummy_types.hpp
    include/dummyrpg/floor.hpp
    include/dummyrpg/layer.hpp
    include/dummyrpg/map.hpp
)

set(LIB_SOURCES 
    src/floor.cpp
    src/map.cpp
)

add_library(dummyrpg STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Add compilation warnings
if(MSVC)
  target_compile_options(dummyrpg PRIVATE /W4 /W14640)
else()
  target_compile_options(dummyrpg PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif()


add_executable(main_tests
    tests/main_tests.cpp
)
target_link_libraries(main_tests dummyrpg)
