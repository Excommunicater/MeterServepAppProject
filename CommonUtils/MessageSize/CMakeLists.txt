cmake_minimum_required(VERSION 3.16.3)

set(thisProject messageSize)

add_library(${thisProject} 
    messageSize.c
)

target_include_directories(${thisProject}
    PUBLIC ../../commonIncludes/ 
)