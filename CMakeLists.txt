cmake_minimum_required(VERSION 3.26)
project(rapic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/rapic/rest_api_client.cpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(tests)
