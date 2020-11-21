cmake_minimum_required(VERSION 3.16.3)

set(thisProject simpleGet)

add_executable(${thisProject} 
    simpleGetTest.c
)

target_link_libraries(${thisProject} 
    testUtils
)