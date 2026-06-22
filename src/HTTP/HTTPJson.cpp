#include "../../include/Errors/AppErrors.h"
#include "../../include/HTTP/HTTPJson.h"

using json = nlohmann::json;

json HTTPJson::parseObjson(const HTTPRequest& request)
{
    try
    {
        json body = json::parse(request.body);
        if(!body.is_object())
        {
            throw ValidationError("body is not object json");
        }

        return body;
    }
    catch(const json::parse_error&)
    {
        throw InvalidJson("Invalid json body");
    } 
}

std::string HTTPJson::requireString(
    const json& body,
    std::string namePath
)
{
    if(!body.contains(namePath))
    {
        throw ValidationError("body is not parametr");
    }
    if(!body[namePath].is_string())
    {
        throw ValidationError("parametr is not string");
    }
    std::string path = body[namePath];
    if(path.empty())
    {
        throw ValidationError("parametn is empty");
    }
    return path;
}

int HTTPJson::requireInt(
    const json& body,
    std::string namePath
)
{
    if(!body.contains(namePath))
    {
        throw ValidationError("body is not parametr");
    }
    if(!body[namePath].is_number_integer())
    {
        throw ValidationError("parametr is not integer");
    }
    return body[namePath];
}

HTTPResponse HTTPJson::makeResponse(
    int statusCode,
    std::string statusText,
    const json& body
){
    HTTPResponse response;
    response.statusCode = statusCode;
    response.statusText = statusText;

    response.body = body.dump();
    response.headers["Content-Length"] = std::to_string(response.body.size());

    return response;
}
