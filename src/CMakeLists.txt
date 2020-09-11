
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_LIST_DIR}/lib/*.c)


add_library(lib STATIC ${SRC})
target_include_directories(lib PUBLIC ${INCLUDE_DIR})

add_executable(app main.c)
target_link_libraries(app lib pthread)
target_include_directories(app PUBLIC ${INCLUDE_DIR})

add_executable(select_example ./select/main.c)

