#include "../../include/HTTP/HTTPParser.h"
#include <sstream>


HTTPRequest HTTPParser::parse(const std::string & rawRequest)
{
    HTTPRequest request;

    std::istringstream stream(rawRequest);

    std::string line;
    std::getline(stream, line);
    if (!line.empty() && line.back()=='\r')line.pop_back();
    parseRequestLine(line, request);

    parseHeader(stream, request);

    parseBody(stream, request);

    return request;
}

void HTTPParser::parseRequestLine(const std::string &line, HTTPRequest & request)
{
    std::istringstream ss(line);

    ss>>request.method;
    ss>>request.path;
    ss>>request.version;
}

void HTTPParser::parseHeader(std::istringstream& stream, HTTPRequest& request)
{
    std::string line;
    while(std::getline(stream, line))
    {
        if (!line.empty() && line.back()=='\r')line.pop_back();
        if (line.empty())break;

        std::size_t separat = line.find(':');
        std::string key = line.substr(0, separat);
        std::string value = line.substr(separat+2);
        request.headers[key] = value;
    }
}

void HTTPParser::parseBody(std::istringstream& stream, HTTPRequest& request)
{
    std::string line;
    while(std::getline(stream, line))
    {
        if (!line.empty() && line.back()=='\r')line.pop_back();
        request.body+=line;
    }
}