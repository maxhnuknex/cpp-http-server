#pragma once
#include "IUserRepository.h"

#include <optional>

class UserService{
    IUserRepository& repository;
public:
    UserService(IUserRepository& repository);

    std::optional<User> getUserById(int id) const;
    User setUser(UserCreateCommand&);
    bool deleteUser(int id);
};