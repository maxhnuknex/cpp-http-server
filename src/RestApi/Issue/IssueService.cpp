#include "../../../include/RestApi/Issue/IssueService.h"

IssueService::IssueService(UserService& userService, ProjectService& projectService)
    : userService(userService), projectService(projectService)
    {}

std::optional<Issue> IssueService::setIssue(int projectId,
                                int authorId,
                                std::optional<int> assigneeId,
                                const std::string& title,
                                const std::string& description)
{
    std::optional<Project> project = projectService.getProject(projectId);
    if(!project.has_value())return std::nullopt;

    std::optional<User> user = userService.getUserById(authorId);
    if(!user.has_value()) return std::nullopt;

    if(assigneeId.has_value())
    {
        std::optional<User> user = userService.getUserById(*assigneeId);
        if(!user.has_value())return std::nullopt;
    }

    Issue issue{nextId, projectId, authorId,assigneeId, title, description, "open"};
    issues[nextId] = issue;
    nextId++;

    return issue;
}

