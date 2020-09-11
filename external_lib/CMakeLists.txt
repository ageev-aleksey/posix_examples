set(LOGC_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/log.c/src)

add_library(logc STATIC ${LOGC_SRC_DIR}/log.c)
target_include_directories(logc PUBLIC ${LOGC_SRC_DIR})
target_compile_definitions(logc PRIVATE LOG_USE_COLOR)
add_library(LOGC::LIB ALIAS logc)
