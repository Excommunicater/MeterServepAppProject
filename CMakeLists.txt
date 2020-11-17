cmake_minimum_required(VERSION 3.16.3)

project(ServerStart)

enable_testing()

add_executable(${PROJECT_NAME} serverStart.c )

add_subdirectory(ServerUtils)
add_subdirectory(MeterHardware)
add_subdirectory(test)
add_subdirectory(external/googletest)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ServerUtils 
    PUBLIC MeterHardware
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE ServerUtils
    PRIVATE MeterHardware
)

target_link_libraries(${PROJECT_NAME} 
    serverUtils 
    meteringHw
)


