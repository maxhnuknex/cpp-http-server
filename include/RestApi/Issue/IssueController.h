#include "IssueService.h"

#include "../../HTTP/HTTPResponse.h"
#include "../../HTTP/HTTPRequest.h"
#include "../../HTTP/HTTPErrors.h"

class IssueController{
    IssueService& issueService;

public:
    IssueController(IssueService&);

    HTTPResponse createIssue(const HTTPRequest&);
};

