cmake_minimum_required(VERSION 3.10.0)
project(dummy)

# Specify C++ version
set(CMAKE_CXX_STANDARD 17)

###############################################################################

add_subdirectory(dummyRPG) 	# lib and tests
add_subdirectory(player)	# player and UI
