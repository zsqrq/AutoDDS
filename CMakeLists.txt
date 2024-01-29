cmake_minimum_required(VERSION 3.5...3.16)
project(AutoDDS)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)

add_executable(testcompile test/testconpile.cpp)