cmake_minimum_required(VERSION 3.16.3)

# Build support library
add_subdirectory(testUtils)

# Build tests
add_subdirectory(getSingle)
add_subdirectory(setSingle)
add_subdirectory(resetRequest)