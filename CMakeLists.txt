cmake_minimum_required(VERSION 3.5)

project(muduo_log LANGUAGES CXX)

include_directories(/usr/include1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB Headers "*.h")
file(GLOB Sources "*.cpp")

add_executable(muduo_log ${Headers} ${Sources})

target_link_libraries(muduo_log pthread)
