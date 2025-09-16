# Example CMake command line to create project build files:
#
# *** Windows ***
# cmake -G "Visual Studio 17 2022" -A Win32 -B Build -S .
#
# *** Linux ***
# cmake -G "Unix Makefiles" -B Build -S .

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language (C or C++)
project(Allocator VERSION 1.0 LANGUAGES CXX)

# Collect all .cpp and *.h source files in the current directory
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h") #поменял на cmake_current_source_dir было без current

add_library(allocator STATIC Allocator.cpp)
target_include_directories(allocator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add an executable target
add_executable(AllocatorApp ${SOURCES})
