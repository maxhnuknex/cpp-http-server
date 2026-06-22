#include "../../../include/RestApi/Project_Api/ProjectService.h"

ProjectService::ProjectService (
    IUserRepository & userRepository,
    IProjectRepository & projectRepository
)
    : userRepository(userRepository), 
    projectRepository(projectRepository)
    {}

std::optional<Project> ProjectService::createProject(ProjectCreateCommand& command)
{
    std::optional<User> user = userRepository.findById(command.ownerId);
    if(!user.has_value())
    {
        return std::nullopt;
    }

    return projectRepository.createProject(command);
}

std::optional<Project> ProjectService::getProject(int id)
{
    return projectRepository.findById(id);
}

bool ProjectService::deleteProject(int id)
{
    return projectRepository.deleteProject(id);
}