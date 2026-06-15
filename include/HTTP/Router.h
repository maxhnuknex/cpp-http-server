#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPErrors.h"


using Handler = std::function<HTTPResponse(const HTTPRequest&)>;

struct Rout{
    std::string method;
    std::string pathPattern;
    Handler handler;
};

class Router
{
    std::vector<Rout> routs;
    Handler staticHendler;

    bool mathPath(const std::string& patern,
                    const std::string& path,
                    std::unordered_map<std::string, std::string>& pathParam);
public:
    void addRoute(const std::string& method,const std::string& path,Handler handler);
    void addStaticRoute(Handler staticHendler);

    HTTPResponse handle(const HTTPRequest& request);
};