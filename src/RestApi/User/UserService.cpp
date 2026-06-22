#include "../../../include/RestApi/User/UserService.h"
#include "../../../include/Errors/AppErrors.h"

UserService::UserService (IUserRepository& repository)
    : repository(repository)
    {}

std::optional<User> UserService::getUserById(int id) const
{
    return repository.findById(id);
}

User UserService::setUser(UserCreateCommand& command)
{
    if(repository.findByEmail(command.email).has_value())
    {
        throw ConflictError("Conflict email");
    }
    return repository.createUser(command);
}

bool UserService::deleteUser(int id)
{
    return repository.deleteUser(id);
}