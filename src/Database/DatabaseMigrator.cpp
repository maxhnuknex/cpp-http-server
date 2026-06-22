#include "../../include/Database/DatabaseMigrator.h"

#include <stdexcept>
#include <string>

DatabaseMigrator::DatabaseMigrator(Database& database)
    : database{database}
    {}

void DatabaseMigrator::run()
{
    execut(R"SQL(
            CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            username TEXT NOT NULL,
            email TEXT NOT NULL UNIQUE,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        );
    )SQL");

    execut(R"SQL(
        CREATE TABLE IF NOT EXISTS projects (
        id INTEGER PRIMARY KEY,
        ownerId INTEGER NOT NULL,
        name TEXT NOT NULL,
        description TEXT NOT NULL,
        createdAt TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
        updatedAt TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,

        FOREIGN KEY (ownerId)
            REFERENCES users(id)
            ON DELETE RESTRICT
        );
    )SQL");
    execut(R"SQL(
        create table if not exists issues (
            id integer  primary key,
            projectId integer not null,
            authorId integer not null,
            assigneeId integer,

            title text not null,
            description text not null,
            status  text not null,

            createdAt TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,

            FOREIGN KEY (projectId)
                REFERENCES projects(id)
                ON DELETE RESTRICT,

            FOREIGN KEY (authorId)
                REFERENCES users(id)
                ON DELETE RESTRICT,

            FOREIGN KEY (assigneeId)
                REFERENCES users(id)
                ON DELETE SET NULL,

            CHECK (status IN ('open', 'in_progress', 'done', 'closed'))
        );
    )SQL");

}

void DatabaseMigrator::execut(const char*sql)
{
    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        database.connection(),
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if(result!=SQLITE_OK)
    {
        std::string message = "failed migrator: ";

        if(errorMessage!=nullptr)
        {
            message += errorMessage;
            sqlite3_free(errorMessage);
        }

        throw std::runtime_error(message);
    }
}