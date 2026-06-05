#include "../../include/HTTP/Pipeline.h"

using Handler = std::function<HTTPResponse(const HTTPRequest&)>;
using Middleware = std::function<HTTPResponse(const HTTPRequest &, Handler)>;


Pipeline::Pipeline(Handler hendler) : finalHendler{hendler}
    {}

void Pipeline::addMiddleWare(Middleware middleware)
{
    middlewares.push_back(middleware);
}

HTTPResponse Pipeline::hendel(const HTTPRequest& request)
{
    Handler cur = finalHendler;
    for(auto it = middlewares.rbegin();it!=middlewares.rend();it++)
    {
        Middleware middleware = *it;
        Handler next = cur;
        
        cur = [middleware, next] (const HTTPRequest& req)
        {
            return middleware(req, next);
        };
    }

    return cur(request);
}