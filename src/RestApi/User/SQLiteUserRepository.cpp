#include "../../../include/RestApi/User/SQLiteUserRepository.h"

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

User SQLiteUserRepository::createUser(
    const std::string& username,
    const std::string& email
)
{
    const char* sql = R"SQL(
        INSERT INTO users (username, email)
        VALUES (?, ?);
    )SQL";

    sqlite3_stmt* statament;
    int result = sqlite3_prepare_v2(
        database.connection(),
        sql,
        -1,
        &statament,
        nullptr
    );
    
    if(result!=SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(database.connection()));
    }

    sqlite3_bind_text(statament, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statament, 2, email.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(statament);
    if(result!=SQLITE_DONE)
    {
        sqlite3_finalize(statament);
        throw std::runtime_error(sqlite3_errmsg(database.connection()));
    }

    sqlite3_finalize(statament);

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

    sqlite3_stmt* statament = nullptr;
    int result = sqlite3_prepare_v2(
        database.connection(),
        sql,
        -1,
        &statament,
        nullptr
    );

    if(result!= SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(database.connection()));
    }

    sqlite3_bind_int(statament,1,id);

    result = sqlite3_step(statament);

    if(result==SQLITE_ROW)
    {
        User user = toUser(statament);
        sqlite3_finalize(statament);
        return user;
    }
    if(result == SQLITE_DONE)
    {
        sqlite3_finalize(statament);
        return std::nullopt;
    }
    sqlite3_finalize(statament);
    throw std::runtime_error(sqlite3_errmsg(database.connection()));
}

std::optional<User> SQLiteUserRepository::findByEmail(const std::string& email)
{
    const char* sql = R"SQL(
        SELECT id, username, email
        From users
        WHERE email = ?;
    )SQL";

    sqlite3_stmt* statement = nullptr;
    int result = sqlite3_prepare(
        database.connection(),
        sql,
        -1,
        &statement,
        nullptr
    );
    if(result!=SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(database.connection()));
    }

    sqlite3_bind_text(statement, 1, email.c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(statement);
    if(result == SQLITE_ROW)
    {
        User user = toUser(statement);
        sqlite3_finalize(statement);
        return user;
    }
    if(result == SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        return std::nullopt;
    }
    sqlite3_finalize(statement);
    throw std::runtime_error(sqlite3_errmsg(database.connection()));
}

bool SQLiteUserRepository::deleteUser(int id)
{
    const char* sql = R"SQL(
        DELETE FROM users
        WHERE id = ?;
    )SQL";

    sqlite3_stmt* statament = nullptr;
    int result = sqlite3_prepare(
        database.connection(),
        sql,
        -1,
        &statament,
        nullptr
    );
    if(result!=SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(database.connection()));
    }

    sqlite3_bind_int(statament, 1, id);

    result = sqlite3_step(statament);
    if(result != SQLITE_DONE)
    {
        sqlite3_finalize(statament);
        throw std::runtime_error(sqlite3_errmsg(database.connection()));
    }

    int deleteRow = sqlite3_changes(database.connection());

    sqlite3_finalize(statament);
    return deleteRow ==1;
}