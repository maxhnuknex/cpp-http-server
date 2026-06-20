#include "IUserRepository.h"
#include "../../Database/Database.h"

class SQLiteUserRepository: public IUserRepository
{
    Database&database;
    User toUser(sqlite3_stmt*);
public:
    SQLiteUserRepository(Database& );

    User createUser(const std::string&, const std::string&) override;
    std::optional<User> findById(int) override;
    std::optional<User> findByEmail(const std::string&) override;
    bool deleteUser(int ) override;
};