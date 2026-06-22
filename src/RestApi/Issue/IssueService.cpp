#include "../../../include/RestApi/Issue/IssueService.h"

IssueService::IssueService(
    IUserRepository& userRepository,
    IProjectRepository& projectRepositoty,
    IIssueRepository& issueRepository
)
    : userRepository(userRepository),
    projectRepositoty(projectRepositoty),
    issueRepository(issueRepository)
    {}


std::optional<Issue> IssueService::findById(int projectId, int IssueId)
{
    return issueRepository.findIssueId(projectId, IssueId);
}

bool IssueService::deleteIssue(int projectId, int IssueId)
{
    return issueRepository.deleteIssue(projectId, IssueId);
}

std::optional<Issue> IssueService::createIssue(IssueCreateCommand command)
{
    if(!userRepository.findById(command.authorId))
    {
        return std::nullopt;
    }
    if(!projectRepositoty.findById(command.projectId))
    {
        return std::nullopt;
    }
    if(command.assigneeId)
    {
        if(!userRepository.findById(*command.assigneeId))
        {
            return std::nullopt;
        }
    }
    return issueRepository.createIssue(command);
}
