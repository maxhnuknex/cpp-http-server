#include "../../include/RestAPI/TaskService.h"

std::vector<Task> TaskService::getAll()
{
    return tasks;
} 

Task TaskService::create(const std::string& title)
{
    tasks.push_back({nextId, title, false});
    nextId++;
    return tasks.back();
}

bool TaskService::remove(int id)
{
    for(auto it = tasks.begin(); it!= tasks.end(); it++)
    {
        if(it->id == id)
        {
            tasks.erase(it);
            return true;
        }
    }
    return false;
}