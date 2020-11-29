cmake_minimum_required(VERSION 3.16.3)

set(thisProject applicationExample)

add_executable(${thisProject} 
    main.c
)

target_link_libraries(${thisProject} 
    queueUtils
    messagingUtils
    errorHandling
)

target_include_directories(${thisProject}
    PUBLIC ../commonIncludes/ 
    PUBLIC ../CommonUtils/QueueUtils/
    PUBLIC ../CommonUtils/MessagingUtils/
    PUBLIC ../CommonUtils/ErrorHandling/
)