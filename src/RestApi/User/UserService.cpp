#include "../../../include/RestApi/User/UserService.h"

UserService::UserService (IUserRepository& repository)
    : repository(repository)
    {}

std::optional<User> UserService::getUserById(int id) const
{
    return repository.findById(id);
}

User UserService::setUser(const std::string& username,
                            const std::string& email)
{
    return repository.createUser(username, email);
}

bool UserService::deleteUser(int id)
{
    return repository.deleteUser(id);
}