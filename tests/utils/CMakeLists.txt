# Utils library source files
set(TEST_UTILS_SOURCE_FILES
        src/http_rest_server.cpp
        src/https_rest_server.cpp
        src/logger.cpp
        src/synced_execution_context.cpp
        src/thread_pool_execution_context.cpp
)

add_library(${PROJECT_NAME}_test_utils ${TEST_UTILS_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}_test_utils PUBLIC include)
target_link_libraries(${PROJECT_NAME}_test_utils
        PUBLIC
        Boost::asio
        Boost::beast
        OpenSSL::SSL
        OpenSSL::Crypto
        spdlog::spdlog
        PRIVATE
        ${PROJECT_NAME}
)