cmake_minimum_required(VERSION 3.16.3)

set(thisProject server)

add_executable(${thisProject} 
    main.c
    notificationUtils.c
    serverMessageHandlers.c
    serverQueueUtils.c
)


target_link_libraries(${thisProject} 
    messagingUtils
    queueUtils
    meterUtils
)

target_include_directories(${thisProject}
    PUBLIC ../CommonUtils/MessagingUtils/ 
    PUBLIC ../CommonUtils/QueueUtils/ 
    PUBLIC ../MeterUtils/
)