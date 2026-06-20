#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <string>

#include "../include/HTTP/HTTPRequest.h"
#include "../include/HTTP/HTTPResponse.h"
#include "../include/HTTP/HTTPParser.h"
#include "../include/HTTP/Router.h"
#include "../include/HTTP/StaticFileHandler.h"
#include "../include/TCPserver/tcpserver.h"
#include "../include/HTTP/Pipeline.h"

#include "../include/RestApi/User/UserService.h"
#include "../include/RestApi/User/UserController.h"
#include "../include/RestApi/Project_Api/ProjectService.h"
#include "../include/RestApi/Project_Api/ProjectController.h"
#include "../include/RestApi/Issue/IssueService.h"
#include "../include/RestApi/Issue/IssueController.h"

#include "../include/Database/Database.h"
#include "../include/Database/DatabaseMigrator.h"

#include "../include/RestApi/User/SQLiteUserRepository.h"

int main()
{
    Database database("server.db");
    DatabaseMigrator databaseMigrator(database);
    databaseMigrator.run();

    SQLiteUserRepository repository(database);

    Router router;

    UserService userService(repository);
    UserController userController(userService);

    ProjectService projectService(userService);
    ProjectController ProjectController(projectService);

    IssueService issueService(userService, projectService);
    IssueController issueController(issueService);

    router.addRoute("GET", "/users/{id}", [&userController](const HTTPRequest& request)
    {
        return userController.getUser(request);
    });

    router.addRoute("POST", "/users", [&userController](const HTTPRequest& request)
    {
        return userController.createUser(request);
    });

    router.addRoute("DELETE", "/users/{id}", [&userController](const HTTPRequest& request){
        return userController.deleteUser(request);
    });

    router.addRoute("POST", "/projects", [&ProjectController](const HTTPRequest& request){
        return ProjectController.createProjecte(request);
    });

    router.addRoute("POST", "/projects/{project_id}/issue", [&issueController](const HTTPRequest& request){
        return issueController.createIssue(request);
    });

    StaticFileHandler staticFile("public");
    router.addStaticRoute([&staticFile](const HTTPRequest& request) 
        {
            return staticFile.handle(request);
        }
    );

    Pipeline pipeline([&router](const HTTPRequest& req){
        return router.handle(req);
    });

    pipeline.addMiddleWare([](const HTTPRequest& request, Handler next)
    {
        std::cout << "[LOG] " << request.method << " " << request.path << std::endl;

        return next(request);
    });

    TCPserver server(8080);

    server.start([&router, &pipeline](const std::string& rawRequest)
    {
        HTTPParser parser;
        HTTPRequest request = parser.parse(rawRequest);

        HTTPResponse response = pipeline.hendel(request);

        return response.toString();
    });

    return 0;
}