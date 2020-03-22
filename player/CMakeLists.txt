cmake_minimum_required(VERSION 3.10.0)
project(dummy)

# Specify C++ version
set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS graphics audio REQUIRED)

###############################################################################

# Package some resources
set(RESOURCE_FILES
    temp_resources/chip1.png
    Resources/tilemap.frag
    Resources/tilemap.vert
)

add_executable(player
    include/CharacterRender.hpp
    include/GameRender.hpp
    include/MapRender.hpp

    src/main.cpp
    src/CharacterRender.cpp
    src/GameRender.cpp
    src/MapRender.cpp
    "${RESOURCE_FILES}"
)

target_include_directories(player PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(player dummyrpg sfml-graphics)

# Add compilation warnings
if(MSVC)
  target_compile_options(player PRIVATE /W4 /w14640)
else()
  target_compile_options(player PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif()

foreach(RESOURCE_FILE IN LISTS RESOURCE_FILES)
    file(COPY "${RESOURCE_FILE}" DESTINATION "${CMAKE_BINARY_DIR}/Resources")
endforeach()

set_target_properties(player PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
  RESOURCE "${RESOURCE_FILES}"
)
