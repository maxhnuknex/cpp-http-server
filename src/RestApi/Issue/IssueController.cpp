#include "../../../include/RestApi/Issue/IssueController.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

IssueController::IssueController(IssueService& issueService)
    :issueService(issueService)
    {}

HTTPResponse IssueController::createIssue(const HTTPRequest& request)
{
    auto projectIdIt = request.pathParams.find("project_id");
    if(projectIdIt == request.pathParams.end())
    {
        return HTTPErrors::invalidPathParam("project_id");
    }
    int projectId{};
    try
    {
        projectId= std::stoi(projectIdIt->second);
    }
    catch(...)
    {
        return HTTPErrors::invalidPathParam("project_id");
    }
    

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
        return HTTPErrors::invalidJson();
    }

    if(!body.contains("authorId") || 
        !body.contains("title") || 
        !body.contains("description"))
    {
        return HTTPErrors::validationError("is not contains");
    }

    if(!body["title"].is_string() || body["title"].empty() || 
        !body["description"].is_string() || body["description"].empty())
    {
        return HTTPErrors::validationError("is not string or empty");
    }
    std::string title = body["title"];
    std::string description = body["description"];

    if(!body["authorId"].is_number_integer())
    {
        return HTTPErrors::validationError("authorId is not integer");
    }
    int authorId = body["authorId"].get<int>();

    std::optional<int> assigneeId = std::nullopt;
    if(body.contains("assigneeId"))
    {
        if(!body["assigneeId"].is_number_integer())
        {
            return HTTPErrors::validationError("assigneeId is not integer");
        }
        assigneeId = body["assigneeId"].get<int>();
    }

    std::optional<Issue> issue = issueService.setIssue(projectId, authorId,
                                                    assigneeId, title, description);
    if(!issue.has_value())
    {
        return HTTPErrors::notFound();
    }

    HTTPResponse response;
    response.statusCode = 201;
    response.statusText = "Created";

    json responseBody;
    responseBody["id"] = issue->id;
    responseBody["projectId"] = issue->projectId;
    responseBody["authorId"] = issue->authorId;
    if(issue->assigneeId.has_value()) responseBody["assigneeId"] = issue->assigneeId;
    responseBody["title"] = issue->title;
    responseBody["description"] = issue->description;
    response.body = responseBody.dump();

    response.headers["Content-Lenght"] = std::to_string(response.body.size());

    return response;
}