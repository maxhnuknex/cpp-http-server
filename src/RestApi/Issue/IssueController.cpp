#include "../../../include/RestApi/Issue/IssueController.h"
#include "../../../include/Errors/AppErrors.h"
#include "../../../include/HTTP/HTTPJson.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace{
    json issueTojson(const Issue& issue)
    {
        json body;
        body["id"] = issue.id;
        body["projectId"] = issue.projectId;
        body["authorId"] = issue.authorId;
        if(issue.assigneeId.has_value())
        {
            body["assigneeId"] = *issue.assigneeId;
        }
        body["title"] = issue.title;
        body["description"] = issue.description;
        body["status"] = issue.status;
        body["createAt"] = issue.createAt;

        return body;
    }
    int findIdProject(const HTTPRequest& request)
    {
        auto itId = request.pathParams.find("project_id");
        if(itId == request.pathParams.end())
        {
            throw ValidationError("Invalid path parametr: project_id");
        }
        int id{};
        try
        {
            return std::stoi(itId->second);
        }
        catch(const std::exception& e)
        {
            throw ValidationError("Invalid path parametr: project_id");
        }
    }
    int findIdIssue(const HTTPRequest& request)
    {
        auto itId = request.pathParams.find("issue_id");
        if(itId == request.pathParams.end())
        {
            throw ValidationError("Invalid path parametr: issue_id");
        }
        int id{};
        try
        {
            return std::stoi(itId->second);
        }
        catch(const std::exception& e)
        {
            throw ValidationError("Invalid path parametr: issue_id");
        }
    }
}

IssueController::IssueController(IssueService& issueService)
    :issueService(issueService)
    {}

HTTPResponse IssueController::createIssue(const HTTPRequest& request)
{
    int projectId = findIdProject(request);
    
    json body = HTTPJson::parseObjson(request);

    std::optional<int> assigneeId = std::nullopt;
    if(body.contains("assigneeId"))
    {
        if(!body["assigneeId"].is_number_integer())
        {
            return HTTPErrors::validationError("assigneeId is not integer");
        }
        assigneeId = body["assigneeId"].get<int>();
    }
    IssueCreateCommand commamd{
        projectId,
        HTTPJson::requireInt(body, "authorId"),
        assigneeId,
        HTTPJson::requireString(body, "title"),
        HTTPJson::requireString(body, "description"),
    };

    std::optional<Issue> issue = issueService.createIssue(commamd);
    if(!issue.has_value())
    {
        return HTTPErrors::notFound();
    }

    return HTTPJson::makeResponse(
        201,
        "Created",
        issueTojson(*issue)
    );
}

HTTPResponse IssueController::getIssue(const HTTPRequest& request)
{
    int projectId = findIdProject(request);
    int issueId = findIdIssue(request);

    std::optional<Issue> issue= issueService.findById(projectId, issueId);

    if(!issue)
    {
        return HTTPErrors::notFound();
    }

    return HTTPJson::makeResponse(
        200,
        "OK",
        issueTojson(*issue) 
    );
}

HTTPResponse IssueController::deleteIssue(const HTTPRequest& request)
{
    int projectId = findIdProject(request);
    int issueId = findIdIssue(request);

    int result = issueService.deleteIssue(projectId, issueId);

    if(!result)return HTTPErrors::notFound();

    return HTTPJson::makeResponse(
        204,
        "No content",
        ""
    );
}