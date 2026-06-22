#pragma once

#include "Project.h"
#include "../User/IUserRepository.h"
#include "IProjectPepository.h"

#include <optional>

class ProjectService
{
    IUserRepository & userRepository;
    IProjectRepository & projectRepository;

public:
    ProjectService(
        IUserRepository & userRepository,
        IProjectRepository & projectRepository
    );

    std::optional<Project> createProject(ProjectCreateCommand&);
    std::optional<Project> getProject(int id);
    bool deleteProject(int id);
};  