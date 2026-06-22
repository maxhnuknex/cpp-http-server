#include "../../../include/RestApi/Project_Api/SQLiteProjectRepository.h"
#include "../../../include/Errors/AppErrors.h"
#include "../../../include/Database/SqliteStatement.h"

Project SQLiteProjectRepository::toProject(sqlite3_stmt* statement)
{
    Project project;
    project.id = sqlite3_column_int(statement,0);
    project.ownerId = sqlite3_column_int(statement,1);
    
    project.name = reinterpret_cast<const char*> (sqlite3_column_text(statement,2));
    project.description = reinterpret_cast<const char*> (sqlite3_column_text(statement,3));

    project.createdAt = reinterpret_cast<const char*> (sqlite3_column_text(statement,4));
    project.updatedAt = reinterpret_cast<const char*> (sqlite3_column_text(statement,5));

    return project;
}

SQLiteProjectRepository::SQLiteProjectRepository(Database& database)
    : database(database)
    {}

std::optional<Project> SQLiteProjectRepository::createProject(ProjectCreateCommand& command)
{
    const char* sql = R"SQL(
        INSERT INTO projects (ownerId, name, description)
        VALUES (?, ?, ?);
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(), 1, command.ownerId);
    sqlite3_bind_text(
        statement.getStatement(),
        2,
        command.name.c_str(),
        -1,
        SQLITE_TRANSIENT
    );
    sqlite3_bind_text(
        statement.getStatement(),
        3,
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
    return findById(id);
}

std::optional<Project> SQLiteProjectRepository::findById(int id)
{
    const char* sql = R"SQL(
        SELECT id, ownerId, name, description, createdAt, updatedAt
        FROM projects
        WHERE id = ?;
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(), 1, id);

    int result = sqlite3_step(statement.getStatement());

    if(result == SQLITE_ROW)
    {
        return toProject(statement.getStatement());
    }

    if(result == SQLITE_DONE)
    {
        return std::nullopt;
    }
    throw DatabaseError(sqlite3_errmsg(database.connection()));
}

std::vector<Project> SQLiteProjectRepository::findAll()
{
    const char* sql = R"SQL(
        SELECT id, ownerId, name, description, createdAt, updatedAt
        FROM projects
        ORDER BY id;
    )SQL";

    SqliteStatement statement(database, sql);
    std::vector<Project> projects;

    while(true)
    {
        int result = sqlite3_step(statement.getStatement());

        if(result == SQLITE_ROW)
        {
            projects.push_back(toProject(statement.getStatement()));
            continue;
        }

        if(result == SQLITE_DONE)
        {
            return projects;
        }

        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }
}

bool SQLiteProjectRepository::deleteProject(int id)
{
    const char* sql =  R"SQL(
        DELETE
        FROM projects
        WHERE id = ?;
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(), 1, id);

    int result = sqlite3_step(statement.getStatement());

    if(result!= SQLITE_DONE)
    {
        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }

    int deleteRow = sqlite3_changes(database.connection());

    return deleteRow==1;
}
