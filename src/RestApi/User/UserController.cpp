#include "../../../include/RestApi/User/UserController.h"
#include "../../../include/Errors/AppErrors.h"
#include "../../../include/HTTP/HTTPJson.h"
#include <optional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace{
    json userToJson(const User& user)
    {
        json body;
        body["id"] = user.id;
        body["username"] = user.username;
        body["email"] = user.email;
        return body;
    }

    int findIdParam(const HTTPRequest & request)
    {
        auto idIt = request.pathParams.find("id");
        if(idIt == request.pathParams.end())
        {
            throw ValidationError("Invalid path parametr: id");
        }
        try
        {
            return std::stoi(idIt->second);
        }
        catch(...)
        {
            throw ValidationError("Invalid path parametr: id");
        }
    }
}

UserController::UserController (UserService & userService) : userService(userService)
    {}

HTTPResponse UserController::getUser(const HTTPRequest & request)
{
    int id = findIdParam(request);
    
    std::optional<User> user = userService.getUserById(id);

    if(!user.has_value())return HTTPErrors::notFound();
    return HTTPJson::makeResponse(
        200,
        "OK",
        userToJson(*user)
    );
}

HTTPResponse UserController::createUser(const HTTPRequest& request)
{
    json body = HTTPJson::parseObjson(request);

    UserCreateCommand command{
        HTTPJson::requireString(body, "username"),
        HTTPJson::requireString(body, "email")
    };

    User user = userService.setUser(command);

    return HTTPJson::makeResponse(
        201,
        "Created",
        userToJson(user)
    );
}

HTTPResponse UserController::deleteUser(const HTTPRequest& request)
{
    int id = findIdParam(request);

    if(!userService.deleteUser(id))
    {
        return HTTPErrors::notFound();
    }
    json body = json::object();

    return HTTPJson::makeResponse(
        204,
        "No content",
        body
    );
}
