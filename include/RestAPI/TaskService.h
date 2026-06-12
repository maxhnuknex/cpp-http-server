#pragma once

#include "Task.h"
#include <vector>
#include <string>


class TaskService{
    std::vector<Task> tasks;
    int nextId = 1;
public:
    std::vector<Task> getAll();
    Task create(const std::string& title);
    bool remove(int id);
};
