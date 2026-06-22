#pragma once

#include <string>
#include <optional>

struct Issue{
    int id;
    int projectId;
    int authorId;
    std::optional<int> assigneeId;
    std::string title;
    std::string description;
    std::string status;
    std::string createAt;
};

struct IssueCreateCommand
{
    int projectId;
    int authorId;
    std::optional<int> assigneeId;
    std::string title;
    std::string description;
};
