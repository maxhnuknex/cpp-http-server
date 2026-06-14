#pragma once 
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "HTTPErrors.h"

class StaticFileHandler
{
    std::string rootDirectory;

    bool isSafePath(const std::string& path) const;
    std::string buildFilePath(const std::string& path) const;
    std::string readFile(const std::string& filePath) const;

    std::string getContentType(const std::string& filePath)const;
public:
    StaticFileHandler(const std::string& rootDirectory);
    HTTPResponse handle(const HTTPRequest& request);
};