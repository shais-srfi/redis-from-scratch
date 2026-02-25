cmake_minimum_required(VERSION 3.10)

project(redis)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)