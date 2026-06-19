cmake_minimum_required(VERSION 3.20)

project(Server_project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(server
    src/main.cpp
    src/HTTP/HTTPParser.cpp
    src/HTTP/HTTPResponse.cpp
    src/HTTP/Router.cpp
    src/TCPserver/TCPserver.cpp
    src/HTTP/StaticFileHandler.cpp
    src/TCPserver/ThreadPool.cpp
    src/HTTP/Pipeline.cpp
    src/HTTP/HTTPErrors.cpp
    src/RestApi/User/UserController.cpp
    src/RestApi/User/UserService.cpp
    src/RestApi/Project_Api/ProjectController.cpp
    src/RestApi/Project_Api/ProjectService.cpp
    src/RestApi/Issue/IssueController.cpp
    src/RestApi/Issue/IssueService.cpp
)

target_include_directories(server PRIVATE
    include
)

target_link_libraries(server PRIVATE
    nlohmann_json::nlohmann_json
)
