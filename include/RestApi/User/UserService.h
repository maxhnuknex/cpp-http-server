#pragma once
#include "User.h"

#include <unordered_map>
#include <optional>

class UserService{
    std::unordered_map<int, User> users; 
    int nexId=3;
public:
    UserService();

    std::optional<User> getUserById(int id) const;
    User setUser(const std::string& username, const std::string& email);
    bool deleteUser(int id);
};