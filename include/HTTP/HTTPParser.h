#pragma once
#include "HTTPRequest.h"


class HTTPParser
{
    void parseHeader(std::istringstream& stream, HTTPRequest & request);
    void parseBody(std::istringstream& stream, HTTPRequest & request);
    void parseRequestLine(const std::string &line, HTTPRequest & request);
public:

    HTTPRequest parse(const std::string & rawRequest);
};