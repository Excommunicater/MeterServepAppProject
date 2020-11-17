project(ExampleTest)
add_executable(${PROJECT_NAME} test_test.cpp )

target_link_libraries(${PROJECT_NAME} 
    gtest_main
)

add_test(
    NAME ${PROJECT_NAME} 
    COMMAND ${PROJECT_NAME} 
)