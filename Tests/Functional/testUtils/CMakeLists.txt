cmake_minimum_required(VERSION 3.16.3)

set(thisLibrary testUtils)

add_library(${thisLibrary} 
    testUtils.c
)

target_link_libraries(${thisLibrary} 
    serverUtils
)