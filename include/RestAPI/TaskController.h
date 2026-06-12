#pragma once

#include "TaskService.h"
#include "../HTTP/HTTPRequest.h"
#include "../HTTP/HTTPResponse.h"

class TaskController{
    TaskService& service;

    std::string taskToJson(const Task& task);
    std::string tasksToJson(const std::vector<Task>& tasks);
    std::string extractTitle(const std::string& body);
public:
    TaskController(TaskService& service);

    HTTPResponse getTasks(const HTTPRequest& request);
    HTTPResponse creatTask(const HTTPRequest& request);
    HTTPResponse deleteTask(const HTTPRequest& request);
};
