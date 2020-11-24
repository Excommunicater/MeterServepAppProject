cmake_minimum_required(VERSION 3.16.3)

set(thisProject meterUtils)

add_library(${thisProject} 
    instantaneousMeterValues.c
    meterUtils.c
    minMaxValues.c
)

target_include_directories(${thisProject}
    PUBLIC ../commonIncludes/ 
    PUBLIC ../CommonUtils/ErrorHandling
    PUBLIC ../CommonUtils/FileUtils
)

target_link_libraries(${thisProject} 
    fileUtils
    errorHandling
)