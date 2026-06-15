#include "../../include/HTTP/Router.h"
#include <sstream>


void Router::addRoute(
    const std::string& method,
    const std::string& path,
    Handler handler
)
{
    routs.push_back(Rout{method, path, handler});
}

void Router::addStaticRoute(Handler handler) 
{
    staticHendler = handler;
}

static std::vector<std::string> slitPath(const std::string & path)
{
    std::vector<std::string> parts;
    std::istringstream ss(path);
    std::string tmp{};
    
    while(getline(ss, tmp, '/'))
    {
        if(!tmp.empty())parts.push_back(tmp);
    }

    return parts;
}

bool Router::mathPath(const std::string& pattern, 
                const std::string& path,
                std::unordered_map<std::string, std::string>& pathParam)
{
    std::vector<std::string> patternParts = slitPath(pattern);
    std::vector<std::string> pathParts = slitPath(path);

    if(patternParts.size()!= pathParts.size())return false;

    for(size_t i =0; i<patternParts.size();i++)
    {
        std::string patternPart = patternParts[i];
        std::string pathPart = pathParts[i];

        bool isParam = patternPart.size()>=3 &&
                        patternPart.front() == '{' &&
                        patternPart.back() == '}';

        if(isParam)
        {
            std::string paramName = patternPart.substr(1, patternPart.size()-2);
            pathParam[paramName] = pathPart;
            continue;
        }

        if(patternPart!= pathPart)return false;
    }

    return true;
}

HTTPResponse Router::handle(const HTTPRequest& request)
{
    for(const Rout& rout : routs)
    {
        if(rout.method!=request.method)
        {
            continue;
        }

        std::unordered_map<std::string, std::string> pathParam;
        if(mathPath(rout.pathPattern, request.path, pathParam))
        {
            HTTPRequest requestRout;
            requestRout.pathParams = pathParam;

            return rout.handler(requestRout);
        }
    }
    if(staticHendler)return staticHendler(request);

    return HTTPErrors::notFound();
}


