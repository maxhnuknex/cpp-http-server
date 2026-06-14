#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPErrors.h"

using Handler = std::function<HTTPResponse(const HTTPRequest&)>;

class Router
{
    std::unordered_map<std::string, Handler> routes;
    Handler staticHendler;
public:
    void addRoute(const std::string& method,const std::string& path,Handler handler);
    void addStaticRoute(Handler staticHendler);

    HTTPResponse handle(const HTTPRequest& request);
};