#pragma once

#include <sqlite3.h>
#include<string>

class Database{
    sqlite3* db = nullptr;

public:
    Database(const std::string& nameDB);
    ~Database();

    sqlite3* connection();
};