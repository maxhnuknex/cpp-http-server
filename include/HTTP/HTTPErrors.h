#pragma once
#include "HTTPResponse.h"
#include <string>

namespace HTTPErrors
{
    HTTPResponse toJson(int statusCode, const std::string& statusText,
                        const std::string& body);

    HTTPResponse notFound();
    HTTPResponse forbidden();
    HTTPResponse badRequest();
    HTTPResponse methodNotAllowed(std::string method);
}
