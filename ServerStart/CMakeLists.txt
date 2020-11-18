cmake_minimum_required(VERSION 3.16.3)

set(thisProject ServerStart)

add_executable(${thisProject} 
    serverStart.c
)

target_link_libraries(${thisProject} 
    serverUtils
)

