#include "../../include/HTTP/HTTPErrors.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

HTTPResponse HTTPErrors::toJson(int statusCode,
                                const std::string& statusText,
                                const std::string& error,
                                const std::string& code
                                )
{
    HTTPResponse response;
    response.statusCode = statusCode;
    response.statusText = statusText;

    json body;
    body["error"] = error;
    body["code"] = code;

    response.headers["Content-Type"] = "application/json";
    response.body = body.dump();
    response.headers["Content-Length"] = std::to_string(response.body.size());

    return response;
}

HTTPResponse HTTPErrors::notFound(){
    return toJson(404, "Not Found", "Rout not found", "ROUTE_NOT_FOUND");
}

HTTPResponse HTTPErrors::forbidden(){
    return toJson(403, "Forbidden", "Forbidden", "FORBIDDEN");
}

HTTPResponse HTTPErrors::badRequest(){
    return toJson(400, "Bad Request", "Bad Requst", "BAD_REQUEST");
}

HTTPResponse HTTPErrors::invalidJson(){
    return toJson(400, "Bad Request", "Invalid JSON body", "INVALID_JSON");
}

HTTPResponse HTTPErrors::validationError(const std::string& message){
    return toJson(400, "Bad Request", message, "VALIDATION_ERROR");
}

HTTPResponse HTTPErrors::invalidPathParam(const std::string& paramName){
    return toJson(400, "Bad Request", "Invalid path parametr: " + paramName, "IVALID_PATH_PARAM");
}


HTTPResponse HTTPErrors::methodNotAllowed(const std::string method)
{
    HTTPResponse response = toJson(
        405,
        "Method Not Allowed",
        "Method not allowed",
        "METHOD_NOT_ALLOWED"
    );

    response.headers["Allow"] = method;
    return response;
}

HTTPResponse HTTPErrors::unsupportedMediaType()
{
    return toJson(415, "Unsupported Media Type", "Unsupported media type", "UNSUPPORTED_MEDIA_TYPE");
}

HTTPResponse HTTPErrors::conflict(const std::string& message)
{
    return toJson(
        409,
        "Conflict",
        message,
        "CONFLICT"
    );
}

HTTPResponse HTTPErrors::serverError()
{
    return toJson(
        500,
        "Internal server Error",
        "Internal server error",
        "INTERNAL_ERROR"
    );
}