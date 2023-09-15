add_executable(${PROJECT_NAME}_unit base.cpp)

target_link_libraries(${PROJECT_NAME}_unit PRIVATE ${PROJECT_NAME} ${PROJECT_NAME}_test_utils GTest::gtest_main)

gtest_discover_tests(${PROJECT_NAME}_unit)