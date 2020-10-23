cmake_minimum_required(VERSION 3.7)
project(relay)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")


IF (WIN32)  
message(STATUS "这是WIN32")
ELSEIF (APPLE)  
message(STATUS "这是APPLE")
ELSEIF (UNIX)  
message(STATUS "这是UNIX")
ELSE ()
message(STATUS "这是??")
ENDIF ()  

set( MYTBT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
message("MYTBT_ROOT: ${MYTBT_ROOT}")
include_directories(${MYTBT_ROOT}/include)


file(GLOB_RECURSE SRC_LIST "${MYTBT_ROOT}/source/*.c")
set(ALL_SRC_LIST ${ALL_SRC_LIST} ${SRC_LIST})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(relay ${ALL_SRC_LIST})

link_directories(${PROJECT_BINARY_DIR}/lib)

add_subdirectory(test)