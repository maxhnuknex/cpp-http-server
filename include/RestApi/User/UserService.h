#pragma once
#include "IUserRepository.h"

#include <optional>

class UserService{
    IUserRepository& repository;
public:
    UserService(IUserRepository& repository);

    std::optional<User> getUserById(int id) const;
    User setUser(const std::string& username, const std::string& email);
    bool deleteUser(int id);
};