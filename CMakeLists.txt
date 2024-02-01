cmake_minimum_required(VERSION 3.5...3.16)
project(AutoDDS)

set(CMAKE_CXX_STANDARD 20 )
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(testcompile test/testconpile.cpp)

add_executable(testnew test/testnew.cpp)
#target_include_directories(testnew
#    PUBLIC ${CMAKE_SOURCE_DIR}/libs
#    PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty
#)