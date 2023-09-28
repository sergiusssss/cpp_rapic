add_executable(${PROJECT_NAME}_component
        https_rest_client_fake_server_base_test.cpp
        https_rest_client_request_test.cpp
)

target_link_libraries(${PROJECT_NAME}_component PRIVATE ${PROJECT_NAME} ${PROJECT_NAME}_test_utils GTest::gtest_main gmock)

gtest_discover_tests(${PROJECT_NAME}_component)