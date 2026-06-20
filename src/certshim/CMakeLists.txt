cmake_minimum_required(VERSION 3.15)
project(certshim CXX)
set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${CMAKE_SOURCE_DIR}/bin)

add_library(certpatch SHARED certpatch.cpp)
set_target_properties(certpatch PROPERTIES PREFIX "")

add_executable(inject inject.cpp)
