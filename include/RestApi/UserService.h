#pragma once
#include "User.h"

#include <unordered_map>
#include <optional>

class UserService{
    std::unordered_map<int, User> users; 

public:
    UserService();

    std::optional<User> getUserById(int id) const;
};