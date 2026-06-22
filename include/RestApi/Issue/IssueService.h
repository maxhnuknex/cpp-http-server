#pragma once

#include "Issue.h"
#include "../User/IUserRepository.h"
#include "../Project_Api/IProjectPepository.h"
#include "IIssueRepository.h"

#include <optional>


class IssueService{
    IUserRepository& userRepository;
    IProjectRepository& projectRepositoty;
    IIssueRepository& issueRepository;
public:
    IssueService(
        IUserRepository& userRepository,
        IProjectRepository& projectRepositoty,
        IIssueRepository& issueRepository
    );

    std::optional<Issue> createIssue(IssueCreateCommand);
    std::optional<Issue> findById(int projectId, int IssueId);
    bool deleteIssue(int projectId, int IssueId);
};