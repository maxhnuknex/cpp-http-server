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
#include "../include/RestApi/UserService.h"
#include "../include/RestApi/UserController.h"


int main()
{
    Router router;

    UserService userService;
    UserController userController(userService);

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