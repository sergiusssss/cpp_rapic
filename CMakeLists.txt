cmake_minimum_required(VERSION 3.26)
project(rapic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TRACY "Enable Tracy profiler" OFF)
option(ENABLE_TESTS "Enable tests build" ON)
option(ENABLE_BENCHMARKS "Enable benchmark  build" OFF)

message(STATUS "ENABLE_TRACY: " ${ENABLE_TRACY})
message(STATUS "ENABLE_TESTS: " ${ENABLE_TESTS})
message(STATUS "ENABLE_BENCHMARKS: " ${ENABLE_BENCHMARKS})

include(FetchContent)
include(ExternalProject)

# Declaration of all required libraries.
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG 7e635fca68d014934b4af8a1cf874f63989352b7 # v1.12.0
)
FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG 564e2ac16907019696cdaba8a93e3588ec596062 # v1.83.0
)
FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG 897aec5b062664d2485f4f9a213715d2e527e0ca # v0.9.1
)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG f8d7d77 # v1.14.0
)

# Fetching of all required libraries if it's enabled.
#set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
set(EXTERNAL_LIBRARIES OpenSSL::SSL OpenSSL::Crypto)

# Always required
FetchContent_MakeAvailable(spdlog)
list(APPEND EXTERNAL_LIBRARIES spdlog::spdlog)
FetchContent_MakeAvailable(boost)
list(APPEND EXTERNAL_LIBRARIES Boost::asio Boost::beast)

## Tracy
#if (${ENABLE_TRACY})
#    set(TRACY_ENABLE ON)
#else ()
#    set(TRACY_ENABLE OFF)
#endif ()
## Always required but doesn't compile if not enabled
#FetchContent_MakeAvailable(tracy)
#list(APPEND EXTERNAL_LIBRARIES Tracy::TracyClient)

# Google tests
if (${ENABLE_TESTS})
    enable_testing()
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()

# Google benchmark
if (${ENABLE_BENCHMARKS})
endif ()

# Library source files
set(SOURCE_FILES
        #src/rapic/http_rest_client.cpp
        #src/rapic/http_rest_client_impl.cpp
        src/rapic/https_rest_client.cpp
        src/rapic/https_rest_client_impl.cpp
        src/rapic/request_task_producer.cpp
        src/rapic/ssl_stream.cpp
        src/rapic/tcp_stream.cpp
        src/rapic/logger.cpp
)

# Creating library
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Setting parameter for output library
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTERNAL_LIBRARIES})

# Add tests subdirectory only if tests enabled
if (${ENABLE_TESTS})
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
    add_subdirectory(tests)
endif ()
