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

    if(hasColumn("projects", "owner_id"))
    {
        execut(R"SQL(
            PRAGMA foreign_keys = OFF;

            BEGIN TRANSACTION;

            ALTER TABLE projects RENAME TO projects_old;

            CREATE TABLE projects (
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

            INSERT INTO projects (id, ownerId, name, description, createdAt, updatedAt)
            SELECT id, owner_id, name, description, created_at, updated_at
            FROM projects_old;

            DROP TABLE projects_old;

            COMMIT;

            PRAGMA foreign_keys = ON;
        )SQL");
    }
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

bool DatabaseMigrator::hasColumn(const char* tableName, const char* columnName)
{
    std::string sql = "PRAGMA table_info(" + std::string(tableName) + ");";
    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        database.connection(),
        sql.c_str(),
        -1,
        &statement,
        nullptr
    );

    if(result != SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(database.connection()));
    }

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char* nameText = sqlite3_column_text(statement, 1);
        std::string name = nameText
            ? reinterpret_cast<const char*>(nameText)
            : "";

        if(name == columnName)
        {
            sqlite3_finalize(statement);
            return true;
        }
    }

    sqlite3_finalize(statement);
    return false;
}
