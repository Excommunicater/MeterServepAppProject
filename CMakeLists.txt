cmake_minimum_required(VERSION 3.16.3)

set(thisProject PhoenixRozmowaProject)

project(${thisProject} )

# Where to store executables and libraries??
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Libraries to build
add_subdirectory(ServerUtils)

# Separate projects to build
add_subdirectory(ServerStart)
add_subdirectory(MeterHardware)
add_subdirectory(ApplicationStart)

# Tests and test framework
add_subdirectory(external/googletest)
add_subdirectory(test)
