#pragma once
#include "Issue.h"

#include <optional>
#include <vector>

class IIssueRepository{

public:
    ~IIssueRepository() = default;

    virtual std::optional<Issue> createIssue(IssueCreateCommand) = 0;
    virtual std::optional<Issue> findIssueId(int projectId, int IssueId) = 0;
    virtual std::vector<Issue> findAllIssue(int projectId) = 0;
    virtual bool deleteIssue(int projectId, int issueId) = 0;
};
