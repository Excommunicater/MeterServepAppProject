cmake_minimum_required(VERSION 3.16.3)

set(thisProject queueUtils)

add_library(${thisProject} 
    queueUtils.c
)

target_include_directories(${thisProject}
    PUBLIC ../../commonIncludes/ 
    PUBLIC ../ErrorHandling/ 
    PUBLIC ../MessageSize
)

target_link_libraries(${thisProject} 
    errorHandling
    messageSize
)