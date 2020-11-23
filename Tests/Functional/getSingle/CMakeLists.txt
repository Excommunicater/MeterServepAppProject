cmake_minimum_required(VERSION 3.16.3)

set(thisProject getSingle)

add_executable(${thisProject} 
    getSingleTest.c
)

target_link_libraries(${thisProject} 
    testUtils
)