cmake_minimum_required(VERSION 3.4.0)
project(iterator)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
set(APP_SRC iterator.cpp)

add_executable(ITE ${APP_SRC})