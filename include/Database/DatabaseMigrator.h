#pragma once

#include "Database.h"

class DatabaseMigrator{
    Database& database;

    void execut(const char* sql);
    bool hasColumn(const char* tableName, const char* columnName);
public:
    DatabaseMigrator(Database&);

    void run();
};
