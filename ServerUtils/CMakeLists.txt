cmake_minimum_required(VERSION 3.16.3)

set(thisLibrary serverUtils)

add_library(${thisLibrary} serverUtils.c)

target_include_directories(${thisLibrary}
    PUBLIC ../commonIncludes/ 
)