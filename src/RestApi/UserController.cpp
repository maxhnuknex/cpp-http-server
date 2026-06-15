#include "../../include/RestApi/UserController.h"
#include <optional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

UserController::UserController (UserService & userService) : userService(userService)
    {}

HTTPResponse UserController::getUser(const HTTPRequest & request)
{
    auto idIt = request.pathParams.find("id");
    if(idIt == request.pathParams.end())
    {
        return HTTPErrors::badRequest();
    }

    int id = std::stoi(idIt->second);

    std::optional<User> user = userService.getUserById(id);

    if(!user.has_value())return HTTPErrors::notFound();

    json body;
    body["id"] = user->id;
    body["username"] = user->username;
    body["email"] = user->email;

    HTTPResponse response;
    response.body = body.dump();
    return response;
}