#pragma once

#include "Issue.h"
#include "../User/UserService.h"
#include "../Project_Api/ProjectService.h"

#include <unordered_map>


class IssueService{
    UserService& userService;
    ProjectService& projectService;

    std::unordered_map<int, Issue> issues;

    int nextId =1;
public:
    IssueService(UserService&, ProjectService&);

    std::optional<Issue> setIssue(int projectId,
                                int authorId,
                                std::optional<int> assigneeId,
                                const std::string& title,
                                const std::string& description);
};