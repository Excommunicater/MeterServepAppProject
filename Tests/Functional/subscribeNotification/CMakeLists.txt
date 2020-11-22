cmake_minimum_required(VERSION 3.16.3)

set(thisProject subscribeNotification)

add_executable(${thisProject} 
    subscribeNotificationTest.c
)

target_link_libraries(${thisProject} 
    testUtils
)