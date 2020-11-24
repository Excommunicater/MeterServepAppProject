cmake_minimum_required(VERSION 3.16.3)

set(thisLibrary testUtils)

add_library(${thisLibrary} 
    testUtils.c
)

target_link_libraries(${thisLibrary} 
    queueUtils
    messagingUtils
)

target_include_directories(${thisLibrary}
    PUBLIC ../../../commonIncludes/ 
    PUBLIC ../../../CommonUtils/QueueUtils/
    PUBLIC ../../../CommonUtils/MessagingUtils/
)
