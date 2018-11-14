cmake_minimum_required(VERSION 0.3)
project(42sh)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
include_directories(library/include)

set(SOURCE_FILES
    src/main.c)

add_executable(42sh ${SOURCE_FILES})
