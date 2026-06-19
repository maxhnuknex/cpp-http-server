#include "../../../include/RestApi/User/UserService.h"

UserService::UserService ()
{
    users[1] = {1, "Ivan", "ivan@mail.ru"};
    users[2] = {2, "Katy", "katy@mail.ru"};
}

std::optional<User> UserService::getUserById(int id) const
{
    auto it = users.find(id);

    if(it == users.end())
    {
        return std::nullopt;
    }

    return it->second;
}

User UserService::setUser(const std::string& username,
                            const std::string& email)
{
    users[nexId] = User{nexId, username, email};
    nexId++;
    return users[nexId-1];
}

bool UserService::deleteUser(int id)
{
    return users.erase(id)>0;
}