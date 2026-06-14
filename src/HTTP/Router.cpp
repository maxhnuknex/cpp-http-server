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
    
    
    std::string line = request.method + " " + request.path;

    auto it = routes.find(line);

    if (it != routes.end())
    {
        return it->second(request);
    }
    if (staticHendler)
    {
        return staticHendler(request);
    }

    return HTTPErrors::notFound();
}
