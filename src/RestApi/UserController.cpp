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
        return HTTPErrors::invalidPathParam("id");
    }

    int id{};

    try
    {
        id = std::stoi(idIt->second);
    }
    catch(...)
    {
        return HTTPErrors::invalidPathParam("id");
    }
    

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

HTTPResponse UserController::createUser(const HTTPRequest& request)
{
    json body;

    try
    {
        body = json::parse(request.body);
    }
    catch(...)
    {
        return HTTPErrors::invalidJson();
    }

    if(!body.is_object())
    {
        return HTTPErrors::validationError("Request body must be a JSON object");
    }
    if(!body.contains("username") || !body["username"].is_string())
    {
        return HTTPErrors::validationError("Failed username");
    }
    if(!body.contains("email") || !body["email"].is_string()){
        return HTTPErrors::validationError("Failed email");
    }

    std::string username = body["username"];
    std::string email = body["email"];
    if(username.empty() || email.empty())
        return HTTPErrors::validationError("username or email is empty");

    User user = userService.setUser(username, email);

    HTTPResponse response;
    response.statusCode = 201;
    response.statusText = "Created";

    json responseBody;
    responseBody["id"] = user.id;
    responseBody["username"] = user.username;
    responseBody["email"] = user.email;
    response.body = responseBody.dump();

    response.headers["Content-Length"] = std::to_string(response.body.size());

    return response;
}