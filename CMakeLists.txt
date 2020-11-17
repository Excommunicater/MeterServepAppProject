cmake_minimum_required(VERSION 3.16.3)

project(ServerStart)

add_executable(${PROJECT_NAME} serverStart.c )

add_subdirectory(ServerUtils)

target_include_directories(${PROJECT_NAME} PUBLIC ServerUtils/)

target_link_directories(${PROJECT_NAME} PRIVATE ServerUtils)

target_link_libraries(${PROJECT_NAME} serverUtils)
