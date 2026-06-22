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
#include "../include/RestApi/Project_Api/SQLiteProjectRepository.h"
#include "../include/RestApi/Issue/SQLiteIssueRepository.h"

#include "../include/HTTP/HTTPErrors.h"
#include "../include/Errors/AppErrors.h"

int main()
{
    Database database("server.db");
    DatabaseMigrator databaseMigrator(database);
    databaseMigrator.run();

    SQLiteUserRepository userRepository(database);
    SQLiteProjectRepository projectRepository(database);
    SQLiteIssueRepository issueRepository(database);

    Router router;

    UserService userService(userRepository);
    UserController userController(userService);

    ProjectService projectService(userRepository, projectRepository);
    ProjectController projectController(projectService);

    IssueService issueService(userRepository, projectRepository, issueRepository);
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

    router.addRoute("POST", "/projects", [&projectController](const HTTPRequest& request){
        return projectController.createProjecte(request);
    });
    router.addRoute("GET", "/projects/{project_id}", [&projectController](const HTTPRequest& request){
        return projectController.getProject(request);
    });
    router.addRoute("DELETE", "/projects/{project_id}", [&projectController](const HTTPRequest& request){
        return projectController.deleteProject(request);
    });


    router.addRoute("POST", "/projects/{project_id}/issues", [&issueController](const HTTPRequest& request){
        return issueController.createIssue(request);
    });
    router.addRoute("GET", "/projects/{project_id}/issues/{issue_id}", [&issueController](const HTTPRequest& request){
        return issueController.getIssue(request);
    });
    router.addRoute("DELETE", "/projects/{project_id}/issues/{issue_id}", [&issueController](const HTTPRequest& request){
        return issueController.deleteIssue(request);
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

    pipeline.addMiddleWare([](const HTTPRequest& request, Handler next){
        try
        {
            return next(request);
        }
        catch(const ConflictError& error)
        {
            return HTTPErrors::conflict(error.what());
        }
        catch(const DatabaseError& error)
        {
            std::cerr << "[DATABASE ERROR] " << error.what() << std::endl;

            return HTTPErrors::serverError();
        }
        catch(const ValidationError& error)
        {
            return HTTPErrors::validationError(error.what());
        }
        catch(const InvalidJson& error)
        {
            return HTTPErrors::invalidJson();
        }
        catch(const std::exception& error)
        {
            std::cerr << "[UNEXPECTED ERROR] " << error.what() << std::endl;

            return HTTPErrors::serverError();
        }
        
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