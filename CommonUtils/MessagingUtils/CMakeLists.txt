cmake_minimum_required(VERSION 3.16.3)

set(thisProject messagingUtils)

add_library(${thisProject} 
    messagingUtils.c
)

target_include_directories(${thisProject}
    PUBLIC ../../commonIncludes/ 
    PUBLIC ../QueueUtils
)

target_link_libraries(${thisProject} 
    queueUtils
)