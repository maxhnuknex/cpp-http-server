#pragma once
#include "Database.h"
#include <sqlite3.h>

class SqliteStatement{
    sqlite3_stmt* statement = nullptr;

public:
    SqliteStatement(Database& Database, const char* sql);
    ~SqliteStatement();

    sqlite3_stmt* getStatement() const;
};

