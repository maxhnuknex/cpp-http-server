#include <vector>
#include <functional>

#include "HTTPRequest.h"
#include "HTTPResponse.h"

using Handler = std::function<HTTPResponse(const HTTPRequest&)>;
using Middleware = std::function<HTTPResponse(const HTTPRequest &, Handler)>;

class Pipeline{
    Handler finalHendler;
    std::vector<Middleware> middlewares;

public:
    Pipeline(Handler);

    void addMiddleWare(Middleware middleware);

    HTTPResponse hendel(const HTTPRequest&);

};
