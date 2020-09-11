cmake_minimum_required(VERSION 3.10)

project(sockets_threads)

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(EXTERNAL_LIB ${CMAKE_CURRENT_LIST_DIR}/external_lib)

add_subdirectory(${EXTERNAL_LIB})
add_subdirectory(${SOURCE_DIR})