#include "../../include/HTTP/StaticFileHandler.h"
#include "../../include/HTTP/HTTPErrors.h"

HTTPResponse StaticFileHandler::handle(const HTTPRequest& request)
{
    if (request.method !="GET")
    {
        return HTTPErrors::methodNotAllowed("GET");
    }
    if(!isSafePath(request.path))
    {
        return HTTPErrors::forbidden();
    }

    std::string filePath = buildFilePath(request.path);
    if (!std::filesystem::exists(filePath))
    {
        return HTTPErrors::notFound();
    }

    std::string body = readFile(filePath);

    HTTPResponse response;
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
