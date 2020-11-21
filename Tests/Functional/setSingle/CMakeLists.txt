cmake_minimum_required(VERSION 3.16.3)

set(thisProject setSingle)

add_executable(${thisProject} 
    setSingleTest.c
)

target_link_libraries(${thisProject} 
    testUtils
)