#include "../../include/HTTP/HTTPErrors.h"

HTTPResponse HTTPErrors::toJson(int statusCode,
                                const std::string& statusText,
                                const std::string& body)
{
    HTTPResponse response;
    response.statusCode = statusCode;
    response.statusText = statusText;

    response.body = body;
    response.headers["Content-Type"] = "application/json";
    response.headers["Content-Length"] = std::to_string(body.size());

    return response;
}

HTTPResponse HTTPErrors::notFound(){
    return toJson(404, "Not Found", "{\"error\":\"Route not found\",\"code\":\"ROUTE_NOT_FOUND\"}");
}

HTTPResponse HTTPErrors::forbidden(){
    return toJson(403, "Forbidden", "{\"error\":\"Forbidden\",\"code\":\"FORBIDDEN\"}");
}

HTTPResponse HTTPErrors::badRequest(){
    return toJson(400, "Bad Request", "{\"error\":\"Bad request\",\"code\":\"BAD_REQUEST\"}");
}

HTTPResponse HTTPErrors::methodNotAllowed(std::string method){
    HTTPResponse response =  toJson(405, "Method Not Allowed", "{\"error\":\"Method not allowed\",\"code\":\"METHOD_NOT_ALLOWED\"}");

    response.headers["Allow"] = method;
    return response;
}