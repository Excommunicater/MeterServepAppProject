cmake_minimum_required(VERSION 3.16.3)

set(thisProject fileUtils)

add_library(${thisProject} 
    fileUtils.c
)

target_include_directories(${thisProject}
    PUBLIC ../ErrorHandling/ 
)

target_link_libraries(${thisProject} 
    errorHandling
)