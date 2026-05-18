cmake_minimum_required(VERSION 3.20)

project(Server_project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server
    src/main.cpp
)