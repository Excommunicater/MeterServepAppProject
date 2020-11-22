cmake_minimum_required(VERSION 3.16.3)

set(thisProject ServerStart)

add_executable(${thisProject} 
    serverStart.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")

target_link_libraries(${thisProject} 
    serverUtils
)


