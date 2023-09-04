add_executable(${PROJECT_NAME}_component base.cpp)

target_link_libraries(${PROJECT_NAME}_component PRIVATE ${PROJECT_NAME} GTest::gtest_main)

gtest_discover_tests(${PROJECT_NAME}_component)