#pragma once
#include "HTTPResponse.h"
#include <string>

namespace HTTPErrors
{
    HTTPResponse toJson(int statusCode, const std::string& statusText,
                        const std::string& error, const std::string & code);

    HTTPResponse notFound();
    HTTPResponse forbidden();
    HTTPResponse badRequest();
    HTTPResponse methodNotAllowed(std::string method);

    HTTPResponse invalidJson();
    HTTPResponse validationError(const std::string& message);
    HTTPResponse invalidPathParam(const std::string& paramName);
    HTTPResponse unsupportedMediaType();
}
