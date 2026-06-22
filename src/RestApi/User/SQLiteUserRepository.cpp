#include "../../../include/RestApi/User/SQLiteUserRepository.h"
#include "../../../include/Errors/AppErrors.h"
#include "../../../include/Database/SqliteStatement.h"

SQLiteUserRepository::SQLiteUserRepository (Database& database)
    : database(database)
    {}

User SQLiteUserRepository::toUser(sqlite3_stmt* statement)
{
    User user;

    user.id = sqlite3_column_int(statement, 0);

    const unsigned char* usernameText = sqlite3_column_text(statement, 1);
    const unsigned char* emailText = sqlite3_column_text(statement, 2);

    user.username = usernameText
        ? reinterpret_cast<const char*>(usernameText)
        : "";
    user.email = emailText
        ? reinterpret_cast<const char*>(emailText)
        : "";
    
    return user;
}

User SQLiteUserRepository::createUser(UserCreateCommand& command)
{
    const std::string& username = command.username;
    const std::string& email = command.email;
    const char* sql = R"SQL(
        INSERT INTO users (username, email)
        VALUES (?, ?);
    )SQL";

    SqliteStatement statement(database,sql);

    sqlite3_bind_text(statement.getStatement(), 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement.getStatement(), 2, email.c_str(), -1, SQLITE_TRANSIENT);

    int result = sqlite3_step(statement.getStatement());
    if(result!=SQLITE_DONE)
    {
        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }

    int id = static_cast<int>(sqlite3_last_insert_rowid(database.connection()));

    return User{id, username, email};
}

std::optional<User> SQLiteUserRepository::findById(int id)
{
    const char* sql = R"SQL(
        SELECT id, username, email
        FROM users
        WHERE id=?;
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(),1,id);

    int result = sqlite3_step(statement.getStatement());

    if(result==SQLITE_ROW)
    {
        User user = toUser(statement.getStatement());
        return user;
    }
    if(result == SQLITE_DONE)
    {
        return std::nullopt;
    }
    throw DatabaseError(sqlite3_errmsg(database.connection()));
}

std::optional<User> SQLiteUserRepository::findByEmail(const std::string& email)
{
    const char* sql = R"SQL(
        SELECT id, username, email
        From users
        WHERE email = ?;
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_text(statement.getStatement(), 1, email.c_str(), -1, SQLITE_TRANSIENT);

    int result = sqlite3_step(statement.getStatement());
    if(result == SQLITE_ROW)
    {
        User user = toUser(statement.getStatement());
        return user;
    }
    if(result == SQLITE_DONE)
    {
        return std::nullopt;
    }
    throw DatabaseError(sqlite3_errmsg(database.connection()));
}

bool SQLiteUserRepository::deleteUser(int id)
{
    const char* sql = R"SQL(
        DELETE FROM users
        WHERE id = ?;
    )SQL";

    SqliteStatement statement(database, sql);

    sqlite3_bind_int(statement.getStatement(), 1, id);

    int result = sqlite3_step(statement.getStatement());
    if(result != SQLITE_DONE)
    {
        throw DatabaseError(sqlite3_errmsg(database.connection()));
    }

    int deleteRow = sqlite3_changes(database.connection());
    return deleteRow ==1;
}