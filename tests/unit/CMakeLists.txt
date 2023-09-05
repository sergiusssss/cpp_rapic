add_executable(${PROJECT_NAME}_unit thread_pool_context_test.cpp)

target_link_libraries(${PROJECT_NAME}_unit PRIVATE ${PROJECT_NAME} GTest::gtest_main)

gtest_discover_tests(${PROJECT_NAME}_unit)