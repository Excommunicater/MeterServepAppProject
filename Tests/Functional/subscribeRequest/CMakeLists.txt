cmake_minimum_required(VERSION 3.16.3)

set(thisProject subscribeRequest)

add_executable(${thisProject} 
    subscribeRequestTest.c
)

target_link_libraries(${thisProject} 
    testUtils
)