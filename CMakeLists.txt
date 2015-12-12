cmake_minimum_required(VERSION 3.2)
project(Malloc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(Malloc ${SOURCE_FILES})