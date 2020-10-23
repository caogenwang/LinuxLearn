set(APP_SRC main.c)

set( MYTBT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${MYTBT_ROOT}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(test ${APP_SRC})

link_directories(${PROJECT_BINARY_DIR}/lib)

set(LIB_DIR ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

target_link_libraries(test
relay
)