#include "../../../include/RestApi/Project_Api/ProjectController.h"
#include "../../../include/Errors/AppErrors.h"
#include "../../../include/HTTP/HTTPJson.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace{
    json projectToJson(const Project& project)
    {
        json body;
        body["id"] = project.id;
        body["ownerId"] = project.ownerId;
        body["name"] = project.name;
        body["description"] = project.description;
        body["createdAt"] = project.createdAt;
        body["updatedAt"] = project.updatedAt;

        return body;
    }
    int findIdParam(const HTTPRequest& request)
    {
        auto itId = request.pathParams.find("id");
        if(itId == request.pathParams.end())
        {
            throw ValidationError("Invalid path parametr: id");
        }
        int id{};
        try
        {
            return std::stoi(itId->second);
        }
        catch(const std::exception& e)
        {
            throw ValidationError("Invalid path parametr: id");
        }
    }
}

ProjectController::ProjectController(ProjectService& projectService)
    : projectService(projectService)
    {}

HTTPResponse ProjectController::getProject(const HTTPRequest& request)
{
    int id = findIdParam(request);

    std::optional<Project> project = projectService.getProject(id);

    if(!project.has_value())
    {
        return HTTPErrors::notFound();
    }
    return HTTPJson::makeResponse(
        200,
        "OK",
        projectToJson(*project)
    );
}

HTTPResponse ProjectController::createProjecte(const HTTPRequest& request)
{
    json body = HTTPJson::parseObjson(request);

    ProjectCreateCommand command {
        HTTPJson::requireInt(body, "ownerId"),
        HTTPJson::requireString(body, "name"),
        HTTPJson::requireString(body, "description")
    };

    std::optional<Project> project = projectService.createProject(command);

    if(!project.has_value())
    {
        return HTTPErrors::notFound();
    }
    return HTTPJson::makeResponse(
        201,
        "Created",
        projectToJson(*project)
    );
}

HTTPResponse ProjectController::deleteProject(const HTTPRequest& request)
{
    int id = findIdParam(request);

    if(!projectService.deleteProject(id))
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
