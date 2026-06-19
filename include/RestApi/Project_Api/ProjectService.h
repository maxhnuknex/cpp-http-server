#pragma once

#include "Project.h"
#include "../User/UserService.h"

#include <unordered_map>
#include <optional>

class ProjectService
{
    UserService& userService;
    std::unordered_map<int, Project> projects;
    int nextId = 1;

public:
    ProjectService(UserService& userService);

    std::optional<Project> getProject(int id);
    std::optional<Project> setProject(int ownerId,
                                        const std::string& name,
                                        const std::string& description);
};  