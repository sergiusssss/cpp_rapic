cmake_minimum_required(VERSION 3.26)
project(rapic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Enable tests" ON)

include(FetchContent)

set(SOURCE_FILES
        src/rapic/http_rest_client.cpp
        src/rapic/thread_pool_context.cpp
        src/rapic/thread_pool_context_impl.cpp
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG 7e635fca68d014934b4af8a1cf874f63989352b7 # v1.12.0
)
FetchContent_MakeAvailable(spdlog)


add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

if (${ENABLE_TESTS})
    enable_testing()
endif ()

add_subdirectory(tests)
