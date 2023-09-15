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


#
#set(OPENSSL_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl-src)
#set(OPENSSL_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl)
#set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_DIR}/include)
#set(OPENSSL_CONFIGURE_COMMAND perl ${OPENSSL_SOURCE_DIR}/Configure)
#ExternalProject_Add(
#        OpenSSL
#        SOURCE_DIR ${OPENSSL_SOURCE_DIR}
#        GIT_REPOSITORY https://github.com/openssl/openssl.git
#        GIT_TAG openssl-3.1.2
#        CONFIGURE_COMMAND
#        ${OPENSSL_CONFIGURE_COMMAND}
#        --prefix=${OPENSSL_INSTALL_DIR}
#        --openssldir=${OPENSSL_INSTALL_DIR}
#        BUILD_COMMAND make
#        TEST_COMMAND ""
#        INSTALL_COMMAND make install
#        INSTALL_DIR ${OPENSSL_INSTALL_DIR}
#)
#
#file(MAKE_DIRECTORY ${OPENSSL_INCLUDE_DIR})
#
#add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
#set_property(TARGET OpenSSL::SSL PROPERTY IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libssl.${OPENSSL_LIBRARY_SUFFIX})
#set_property(TARGET OpenSSL::SSL PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR})
#add_dependencies(OpenSSL::SSL OpenSSL)
#
#add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
#set_property(TARGET OpenSSL::Crypto PROPERTY IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libcrypto.${OPENSSL_LIBRARY_SUFFIX})
#set_property(TARGET OpenSSL::Crypto PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR})
#add_dependencies(OpenSSL::Crypto OpenSSL)

# Fetching of all required libraries if it's enabled.
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
set(EXTERNAL_LIBRARIES OpenSSL::SSL OpenSSL::Crypto)
message(STATUS ${OPENSSL_INCLUDE_DIR})
message(STATUS ${OPENSSL_CRYPTO_LIBRARY})
message(STATUS ${OPENSSL_SSL_LIBRARY})
message(STATUS ${OPENSSL_VERSION})

# Always required
FetchContent_MakeAvailable(spdlog)
list(APPEND EXTERNAL_LIBRARIES spdlog::spdlog)
FetchContent_MakeAvailable(boost)
list(APPEND EXTERNAL_LIBRARIES Boost::asio Boost::beast)

# Tracy
if (${ENABLE_TRACY})
    set(TRACY_ENABLE ON)
else ()
    set(TRACY_ENABLE OFF)
endif ()
# Always required but doesn't compile if not enabled
FetchContent_MakeAvailable(tracy)
list(APPEND EXTERNAL_LIBRARIES Tracy::TracyClient)

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
        # src/rapic/boost_impl.cpp Add BOOST_ASIO_SEPARATE_COMPILATION
        src/rapic/http_rest_client.cpp
        src/rapic/http_request_task.cpp
        src/rapic/http_rest_client_impl.cpp
        src/rapic/https_rest_client.cpp
        src/rapic/https_request_task.cpp
        src/rapic/https_rest_client_impl.cpp
)

# Creating library
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Setting parameter for output library
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTERNAL_LIBRARIES})

# Add tests subdirectory only if tests enabled
if (${ENABLE_TESTS})
    add_subdirectory(tests)
endif ()
