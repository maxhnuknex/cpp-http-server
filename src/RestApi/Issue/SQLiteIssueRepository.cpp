#include "../../../include/RestApi/Issue/SQLiteIssueRepository.h"
#include "../../../include/Errors/AppErrors.h"
#include "../../../include/Database/SqliteStatement.h"

Issue SQLiteIssueRepository::toIssue(sqlite3_stmt* statement)
{
    Issue issue;

    issue.id = sqlite3_column_int(statement, 0);
    issue.projectId = sqlite3_column_int(statement, 1);
    issue.authorId = sqlite3_column_int(statement, 2);

    if(sqlite3_column_type(statement, 3) == SQLITE_NULL)
    {
        issue.assigneeId = std::nullopt;
    }
    else
    {
        issue.assigneeId = sqlite3_column_int(statement, 3);
    }

    issue.title = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
    issue.description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
    issue.status = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
    issue.createAt = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));

    return issue;
}

SQLiteIssueRepository::SQLiteIssueRepository(Database& database)
    : database(database)
    {}

std::optional<Issue> SQLiteIssueRepository::createIssue(IssueCreateCommand command)
{
    const char* sql = R"SQL(
        INSERT INTO issues (projectId, authorId, assigneeId, title, description, status)
        VALUES (?, ?, ?, ?, ?, 'open');
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(), 1, command.projectId);
    sqlite3_bind_int(statement.getStatement(), 2, command.authorId);

    if(command.assigneeId.has_value())
    {
        sqlite3_bind_int(statement.getStatement(), 3, *command.assigneeId);
    }
    else
    {
        sqlite3_bind_null(statement.getStatement(), 3);
    }

    sqlite3_bind_text(
        statement.getStatement(),
        4,
        command.title.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_text(
        statement.getStatement(),
        5,
        command.description.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    int result = sqlite3_step(statement.getStatement());

    if(result != SQLITE_DONE)
    {
        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }

    int id = static_cast<int>(sqlite3_last_insert_rowid(database.connection()));
    return findIssueId(command.projectId, id);
}

std::optional<Issue> SQLiteIssueRepository::findIssueId(int projectId, int issueId)
{
    const char* sql = R"SQL(
        SELECT id, projectId, authorId, assigneeId, title, description, status, createdAt
        FROM issues
        WHERE projectId = ? AND id = ?;
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(), 1, projectId);
    sqlite3_bind_int(statement.getStatement(), 2, issueId);

    int result = sqlite3_step(statement.getStatement());

    if(result == SQLITE_ROW)
    {
        return toIssue(statement.getStatement());
    }

    if(result == SQLITE_DONE)
    {
        return std::nullopt;
    }

    throw DatabaseError(sqlite3_errmsg(database.connection()));
}

std::vector<Issue> SQLiteIssueRepository::findAllIssue(int projectId)
{
    const char* sql = R"SQL(
        SELECT id, projectId, authorId, assigneeId, title, description, status, createdAt
        FROM issues
        WHERE projectId = ?
        ORDER BY id;
    )SQL";

    SqliteStatement statement(database, sql);
    std::vector<Issue> issues;

    sqlite3_bind_int(statement.getStatement(), 1, projectId);

    while(true)
    {
        int result = sqlite3_step(statement.getStatement());

        if(result == SQLITE_ROW)
        {
            issues.push_back(toIssue(statement.getStatement()));
            continue;
        }

        if(result == SQLITE_DONE)
        {
            return issues;
        }

        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }
}

bool SQLiteIssueRepository::deleteIssue(int projectId, int issueId)
{
    const char* sql = R"SQL(
        DELETE
        FROM issues
        WHERE projectId = ? AND id = ?;
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(), 1, projectId);
    sqlite3_bind_int(statement.getStatement(), 2, issueId);

    int result = sqlite3_step(statement.getStatement());

    if(result != SQLITE_DONE)
    {
        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }

    int deleteRow = sqlite3_changes(database.connection());
    return deleteRow == 1;
}
