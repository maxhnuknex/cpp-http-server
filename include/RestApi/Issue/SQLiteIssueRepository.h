#pragma once

#include "IIssueRepository.h"
#include "../../Database/Database.h"

class SQLiteIssueRepository: public IIssueRepository
{
    Database& database;
    Issue toIssue(sqlite3_stmt*);

public:
    SQLiteIssueRepository(Database& database);

    std::optional<Issue> createIssue(IssueCreateCommand command) override;
    std::optional<Issue> findIssueId(int projectId, int issueId) override;
    std::vector<Issue> findAllIssue(int projectId) override;
    bool deleteIssue(int projectId, int issueId) override;
};
