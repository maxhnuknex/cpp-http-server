#pragma once

#include "Database.h"

class DatabaseMigrator{
    Database& database;

    void execut(const char* sql);
public:
    DatabaseMigrator(Database&);

    void run();
};