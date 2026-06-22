#pragma once
#include <string>

struct Project
{
    int id;
    int ownerId;
    std::string name;
    std::string description;
    std::string createdAt;
    std::string updatedAt;
};

struct ProjectCreateCommand
{
    int ownerId;
    std::string name;
    std::string description;
};
