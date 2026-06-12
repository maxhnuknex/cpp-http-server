#include "../../include/HTTP/Router.h"


void Router::addRoute(
    const std::string& method,
    const std::string& path,
    Handler handler
)
{
    std::string key = method + " " + path;
    routes[key] = handler;
}

void Router::addStaticRoute(Handler handler) 
{
    staticHendler = handler;
}

HTTPResponse Router::handle(const HTTPRequest& request)
{
    if(request.method == "DELETE")
    {
        auto it = routes.find(request.method + " " + "/tasks/");
        return it->second(request);
    }
    std::string line = request.method + " " + request.path;

    auto it = routes.find(line);

    if (it != routes.end())
    {
        return it->second(request);
    }
    else{
        return staticHendler(request);
    }
    
    HTTPResponse response;
    response.statusCode = 404;
    response.statusText = "Not Found";
    response.body = "Not Found";
    response.headers["Content-Type"] = "text/plain";
    response.headers["Content-Length"] = std::to_string(response.body.size());

    return response;
}