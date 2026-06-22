#pragma once

#include "User.h"

#include <string>
#include <vector>
#include <optional>

class IUserRepository
{
public:
    virtual ~IUserRepository() = default;

    virtual User createUser(UserCreateCommand&) = 0;

    virtual std::optional<User> findById(int id)=0;

    virtual std::optional<User> findByEmail(
        const std::string& email)=0;

    virtual bool deleteUser(int id) = 0;
};
