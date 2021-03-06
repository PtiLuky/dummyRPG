cmake_minimum_required(VERSION 3.10.0)
project(dummy)

# Specify C++ version
set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS graphics audio REQUIRED)

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")
endif()

###############################################################################

# Package some resources
set(RESOURCE_FILES
    Resources/tilemap.frag
    Resources/tilemap.vert
    Resources/images/ClassicRPG_Sheet.png
    Resources/images/bluePx.png
    Resources/fonts/EuropeanTeletext.ttf
    Resources/fonts/PixelOperator8.ttf
)

set(PLAYER_FILES
    include/ControlGame.hpp
    include/Keymap.hpp
    include/RenderCharacter.hpp
    include/RenderDialog.hpp
    include/RenderGame.hpp
    include/RenderMap.hpp

    src/ControlGame.cpp
    src/RenderCharacter.cpp
    src/RenderDialog.cpp
    src/RenderGame.cpp
    src/RenderMap.cpp
    "${RESOURCE_FILES}")

add_executable(testPlayer
    src/testPlayer.cpp
    "${PLAYER_FILES}"
)

add_executable(player
    src/player.cpp
    "${PLAYER_FILES}"
)

target_include_directories(player PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(testPlayer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(player dummyrpg sfml-graphics)
target_link_libraries(testPlayer dummyrpg sfml-graphics)

# Add compilation warnings
if(MSVC)
  target_compile_options(player PRIVATE /W4 /w14640)
  target_compile_options(testPlayer PRIVATE /W4 /w14640)
else()
  target_compile_options(player PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
  target_compile_options(testPlayer PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)
endif()

foreach(RESOURCE_FILE IN LISTS RESOURCE_FILES)
    get_filename_component(DIR_PATH "${RESOURCE_FILE}" DIRECTORY )
    file(COPY "${RESOURCE_FILE}" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${DIR_PATH})
endforeach()

set_target_properties(player PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
  RESOURCE "${RESOURCE_FILES}"
)
