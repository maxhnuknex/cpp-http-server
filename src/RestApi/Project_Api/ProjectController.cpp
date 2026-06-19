#include "../../../include/RestApi/Project_Api/ProjectController.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

ProjectController::ProjectController(ProjectService& projectService)
    : projectService(projectService)
    {}

HTTPResponse ProjectController::createProjecte(const HTTPRequest& request)
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
        return HTTPErrors::validationError("is not object json");
    }

    if(!body.contains("ownerId") || !body.contains("name") || !body.contains("description"))
    {
        return HTTPErrors::validationError("no parametr");
    }

    if(!body["ownerId"].is_number_integer())
        return HTTPErrors::validationError("parametr is not number");
    if(!body["name"].is_string() || !body["description"].is_string())
    {
        return HTTPErrors::validationError("parametr is not string");
    }
    
    //проверить что это число

    int ownerId = body["ownerId"];
    std::string name = body["name"];
    std::string description = body["description"];

    if(name.empty() || description.empty())return HTTPErrors::validationError("param is empty");

    std::optional<Project> project = projectService.setProject(ownerId, name, description);

    if(!project.has_value())
    {
        return HTTPErrors::notFound();
    }

    json responseBody;
    responseBody["id"] = project->id;
    responseBody["ownerId"] = project->ownerId;
    responseBody["name"] = project->name;
    responseBody["description"] = project->description;

    HTTPResponse response;
    response.statusCode = 201;
    response.statusText = "Created";

    response.body = responseBody.dump();
    response.headers["Content-Lenght"] = std::to_string(response.body.size());

    return response;
}
