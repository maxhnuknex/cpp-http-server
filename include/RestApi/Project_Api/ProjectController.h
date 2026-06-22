#pragma once

#include "ProjectService.h"

#include "../../HTTP/HTTPResponse.h"
#include "../../HTTP/HTTPRequest.h"
#include "../../HTTP/HTTPErrors.h"

class ProjectController{
    ProjectService& projectService;
public:
    ProjectController(ProjectService & projectService);

    HTTPResponse createProjecte(const HTTPRequest& request);
    HTTPResponse getProject(const HTTPRequest& request);
    HTTPResponse deleteProject(const HTTPRequest& request);
};
