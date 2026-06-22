#pragma once
#include "Project.h"

#include <optional>
#include <vector>

class IProjectRepository{

public:
    virtual ~IProjectRepository() = default;

    virtual std::optional<Project> createProject(ProjectCreateCommand&) = 0;
    virtual std::optional<Project> findById(int id) = 0;
    virtual std::vector<Project> findAll()=0;
    virtual bool deleteProject(int id) = 0;
};
