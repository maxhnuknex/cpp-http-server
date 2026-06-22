#pragma once

#include <string>

struct User{
    int id;
    std::string username;
    std::string email;
};

struct UserCreateCommand{
    std::string username;
    std::string email;
};