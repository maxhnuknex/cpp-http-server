#pragma once

#include "HTTPRequest.h"
#include "HTTPResponse.h"

#include <string>
#include <nlohmann/json.hpp>

namespace HTTPJson{
    using json = nlohmann::json;

    json parseObjson(const HTTPRequest&);

    std::string requireString(
        const json&,
        std::string nameParam
    );
    int requireInt(
        const json&,
        std::string namePram
    );

    HTTPResponse makeResponse(
        int,
        std::string,
        const json&
    );
}
