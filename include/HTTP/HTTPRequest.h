#pragma once
#include <string>
#include <unordered_map>

class HTTPRequest
{
public:
    std::string method;
    std::string path;
    std::string version;

    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> pathParams;

    std::string body;
};