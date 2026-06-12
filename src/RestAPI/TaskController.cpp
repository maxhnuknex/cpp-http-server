#include "../../include/RestAPI/TaskController.h"

namespace
{
    HTTPResponse notFound()
    {
        HTTPResponse response;

        response.statusCode = 404;
        response.statusText = "Not Found";
        response.body = "404 Not Found";

        response.headers["Content-Type"] = "text/plain";
        response.headers["Content-Length"] = std::to_string(response.body.size());

        return response;
    }
}

TaskController::TaskController(TaskService& service) : service(service)
    {}

std::string TaskController::taskToJson(const Task& task)
{
    return "{"
        "\"id\":" + std::to_string(task.id) + ","
        "\"title\":\"" + task.title + "\","
        "\"done\":" + (task.done ? "true" : "false") +
    "}";
}
std::string TaskController::tasksToJson(const std::vector<Task>& tasks)
{
    std::string json = "[";

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        json += taskToJson(tasks[i]);

        if (i + 1 < tasks.size())
            json += ",";
    }

    json += "]";
    return json;
}
std::string TaskController::extractTitle(const std::string& body)
{
    size_t colonpos = body.find(":");
    if(colonpos == std::string::npos)return "";

    size_t first = body.find("\"", colonpos);
    if(first == std::string::npos)return "";

    size_t second = body.find("\"", first+1);
    if(second == std::string::npos)return "";

    return body.substr(first+1, second - first-1);
}

HTTPResponse TaskController::getTasks(const HTTPRequest&request)
{
    HTTPResponse response;
    response.headers["Content-Type"] =  "application/json";
    response.body = tasksToJson(service.getAll());

    return response;
}

HTTPResponse TaskController::creatTask(const HTTPRequest& request)
{
    std::string title = extractTitle(request.body);
    service.create(title);

    HTTPResponse response;
    response.statusCode = 201;
    response.statusText = "Created";

    response.headers["Content-Type"] = "application/json";
    response.body = taskToJson(service.getAll().back());

    return response;
}

HTTPResponse TaskController::deleteTask(const HTTPRequest& request)
{
    std::string pref = "/tasks/";
    std::string idSt = request.path.substr(pref.size());
    if(idSt.empty())return notFound();

    int id = std::stoi(idSt);
    bool result = service.remove(id);

    if(result)
    {
        HTTPResponse response;
        response.headers["Content-Type"]= "aplication/json";
        response.body = "{deleted}";

        return response;
    }
    else return notFound();
}
