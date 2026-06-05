#pragma once
#include <string>
#include <unordered_map>

class HTTPResponse
{
public:
    std::string version = "HTTP/1.1";
    int statusCode = 200;
    std::string statusText = "OK";

    std::unordered_map<std::string, std::string> headers;

    std::string body;

    std::string toString() const;
};
