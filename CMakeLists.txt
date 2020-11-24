cmake_minimum_required(VERSION 3.16.3)

set(thisProject PhoenixRozmowaProject)

project(${thisProject} )

# Libraries to build
add_subdirectory(CommonUtils)
add_subdirectory(MeterUtils)

# Separate projects to build
add_subdirectory(Server)
add_subdirectory(MeterSimulator)
add_subdirectory(ApplicationExample)

# Tests and test framework
#add_subdirectory(external/googletest)
add_subdirectory(Tests)
