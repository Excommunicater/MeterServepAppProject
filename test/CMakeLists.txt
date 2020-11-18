cmake_minimum_required(VERSION 3.16.3)

set(thisProject exampleTest)

add_executable(${thisProject} test_test.cpp )

target_link_libraries(${thisProject} 
    gtest_main
    serverUtils
)


add_test(
    NAME ${thisProject} 
    COMMAND ${thisProject} 
)