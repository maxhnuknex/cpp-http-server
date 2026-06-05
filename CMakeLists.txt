cmake_minimum_required(VERSION 3.20)

project(Server_project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(server
    src/main.cpp
    src/HTTP/HTTPParser.cpp
    src/HTTP/HTTPResponse.cpp
    src/HTTP/Router.cpp
    src/TCPserver/TCPserver.cpp
    src/HTTP/StaticFileHandler.cpp
    src/TCPserver/ThreadPool.cpp
    src/HTTP/Pipeline.cpp
)

target_include_directories(server PRIVATE
    include
)