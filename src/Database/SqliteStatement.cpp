#include "../../include/Database/SqliteStatement.h"
#include "../../include/Errors/AppErrors.h"

SqliteStatement::SqliteStatement(Database& database, const char* sql)
{
    int result = sqlite3_prepare_v2(
        database.connection(),
        sql,
        -1,
        &statement,
        nullptr
    );

    if(result!=SQLITE_OK)
    {
        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }
}

SqliteStatement::~SqliteStatement(){
    if(statement!=nullptr)
    {
        sqlite3_finalize(statement);
    }
}


sqlite3_stmt* SqliteStatement::getStatement() const
{
    return statement;
}