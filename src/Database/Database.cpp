#include "../../include/Database/Database.h"

#include <stdexcept>

Database::Database(const std::string& nameDb)
{
    int result = sqlite3_open(nameDb.c_str(), &db);

    if(result != SQLITE_OK)
    {
        std::string message = "failed to open database: ";

        if(db!= nullptr)
        {
            message+= sqlite3_errmsg(db);
            sqlite3_close(db);
            db = nullptr;
        }

        throw std::runtime_error(message);
    }
}

Database::~Database()
{
    if(db!=nullptr)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

sqlite3* Database::connection()
{
    return db;
}