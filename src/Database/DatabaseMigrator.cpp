#include "../../include/Database/DatabaseMigrator.h"

#include <stdexcept>

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