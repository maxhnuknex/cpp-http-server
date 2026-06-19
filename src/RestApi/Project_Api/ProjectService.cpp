#include "../../../include/RestApi/Project_Api/ProjectService.h"

ProjectService::ProjectService (UserService& userService)
    : userService{userService}
    {}

std::optional<Project> ProjectService::setProject(int ownerId,
                                                const std::string& name,
                                                const std::string& disctiption)
{
    std::optional<User> user = userService.getUserById(ownerId);
    if(!user.has_value())
    {
        return std::nullopt;
    }

    Project project{nextId, ownerId, name, disctiption};
    projects[nextId] = project;
    nextId++;

    return projects[nextId-1];
}

std::optional<Project> ProjectService::getProject(int id)
{
    auto it = projects.find(id);
    if(it==projects.end())
    {
        return std::nullopt;
    }

    return it->second;
}