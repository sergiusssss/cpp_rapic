# Utils library source files
set(TEST_UTILS_SOURCE_FILES
        src/synced_execution_context.cpp
        src/thread_pool_execution_context.cpp
)

add_library(${PROJECT_NAME}_test_utils ${TEST_UTILS_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}_test_utils PUBLIC include)
target_link_libraries(${PROJECT_NAME}_test_utils PRIVATE ${PROJECT_NAME})