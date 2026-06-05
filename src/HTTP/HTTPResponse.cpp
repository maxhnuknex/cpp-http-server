#include "../../include/HTTP/HTTPResponse.h"

std::string HTTPResponse::toString() const
{
    std::string line;
    line+= version+ " " + std::to_string(statusCode) + " " + statusText +"\r\n";

    for (auto[key, value] : headers)
    {
        line+= key+": "+value+"\r\n";
    }
    line+="\r\n";

    line+=body;

    return line;
}