#include "../../include/HTTP/StaticFileHandler.h"
namespace{
    HTTPResponse makeMethodNotAllowedResponse()
    {
        HTTPResponse response;

        response.statusCode = 405;
        response.statusText = "Method Not Allowed";
        response.body = "405 Method Not Allowed";

        response.headers["Content-Type"] = "text/plain";
        response.headers["Content-Length"] = std::to_string(response.body.size());

        return response;
    }
    HTTPResponse makeForbiddenResponse()
    {
        HTTPResponse response;

        response.statusCode = 403;
        response.statusText = "Forbidden";
        response.body = "403 Forbidden";

        response.headers["Content-Type"] = "text/plain";
        response.headers["Content-Length"] = std::to_string(response.body.size());

        return response;
    }
    HTTPResponse makeNotFoundResponse()
    {
        HTTPResponse response;

        response.statusCode = 404;
        response.statusText = "Not Found";
        response.body = "404 Not Found";

        response.headers["Content-Type"] = "text/plain";
        response.headers["Content-Length"] = std::to_string(response.body.size());

        return response;
    }
}

HTTPResponse StaticFileHandler::handle(const HTTPRequest& request)
{
    if (request.method !="GET")
    {
        return makeMethodNotAllowedResponse();
    }
    if(!isSafePath(request.path))
    {
        return makeForbiddenResponse();
    }

    std::string filePath = buildFilePath(request.path);
    if (!std::filesystem::exists(filePath))
    {
        return makeNotFoundResponse();
    }

    std::string body = readFile(filePath);

    HTTPResponse response;
    response.statusCode = 200;
    response.statusText ="OK";
    response.body = body;
    response.headers["Content-Type"] = getContentType(filePath);
    response.headers["Content-Length"] = std::to_string(body.size());
    return response;
}

StaticFileHandler::StaticFileHandler(const std::string& rootDirectory) : rootDirectory{rootDirectory}
{
}

bool StaticFileHandler::isSafePath(const std::string& path) const
{
    if (path.empty())return false;

    if (path[0]!='/')return false;

    if (path.find("..")!=std::string::npos)return false;

    return true;
}

std::string StaticFileHandler::buildFilePath(const std::string& path) const
{
    if (path=="/")return rootDirectory + "/index.html";
    return rootDirectory + path;
}

std::string StaticFileHandler::readFile(const std::string& filePath) const
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        return "";
    }

    std::ostringstream buffer;
    buffer<<file.rdbuf();

    return buffer.str();
}

bool hasExtension(const std::string& filePath, const std::string& extension)
{
    if (filePath.size() < extension.size())
    {
        return false;
    }

    return filePath.substr(filePath.size() - extension.size()) == extension;
}

std::string StaticFileHandler::getContentType(const std::string& filePath) const
{
    if (hasExtension(filePath, ".html")) return "text/html";
    if (hasExtension(filePath, ".css")) return "text/css";
    if (hasExtension(filePath, ".js")) return "application/javascript";
    if (hasExtension(filePath, ".png")) return "image/png";
    if (hasExtension(filePath, ".jpg")) return "image/jpeg";
    if (hasExtension(filePath, ".jpeg")) return "image/jpeg";
    if (hasExtension(filePath, ".txt")) return "text/plain";

    return "application/octet-stream";
}