cmake_minimum_required(VERSION 3.16.3)

set(thisProject unitTest)
set(tests serverUtils.cpp)

add_executable(${thisProject} ${tests} )

target_link_libraries(${thisProject} 
    gtest_main
    serverUtils
)


add_test(
    NAME ${thisProject} 
    COMMAND ${thisProject} 
)