cmake_minimum_required(VERSION 3.16.3)

set(thisProject applicationStart)

add_executable(${thisProject} 
    meter_app.c
)

target_link_libraries(${thisProject} 
    serverUtils
)
