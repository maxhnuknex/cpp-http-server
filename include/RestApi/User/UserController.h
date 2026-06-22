#pragma once

#include "UserService.h"

#include "../../HTTP/HTTPResponse.h"
#include "../../HTTP/HTTPRequest.h"
#include "../../HTTP/HTTPErrors.h"

class UserController{
    
    UserService& userService;
public:
    UserController(UserService &);
    
    HTTPResponse getUser(const HTTPRequest& request);
    HTTPResponse createUser(const HTTPRequest& request);
    HTTPResponse deleteUser(const HTTPRequest& request);
};
