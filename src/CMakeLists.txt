
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_LIST_DIR}/lib/*.c)


add_library(lib STATIC ${SRC})
target_include_directories(lib PUBLIC ${INCLUDE_DIR})

add_executable(app main.c fork/main.c echo_server_client/server.c echo_server_client/client.c)
target_link_libraries(app lib pthread)
target_include_directories(app PUBLIC ${INCLUDE_DIR})

add_executable(select_example ./select/main.c)
target_link_libraries(select_example LOGC::LIB)

add_executable(poll_example ./poll/main.c)
target_link_libraries(poll_example LOGC::LIB)

add_executable(signal_example ./signal/main.c)
target_link_libraries(signal_example LOGC::LIB pthread)

add_executable(fork_example ./fork/main.c)
target_link_libraries(fork_example LOGC::LIB)

add_executable(mx_example ./resolve_dns_mx/main.c)
target_link_libraries(mx_example resolv)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/echo_server_client)

