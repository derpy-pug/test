#include "ApiController.h"

#include "../GreetingService.h"

#include <drogon/HttpResponse.h>
#include <json/json.h>
#include <stdexcept>

using namespace drogon;

namespace api
{
namespace
{

HttpResponsePtr makeErrorResponse(HttpStatusCode code, const std::string &message)
{
    Json::Value body;
    body["error"] = message;
    auto resp = HttpResponse::newHttpJsonResponse(body);
    resp->setStatusCode(code);
    return resp;
}

HttpResponsePtr makeGreetingResponse(const std::string &name, HttpStatusCode code)
{
    Json::Value body;
    body["message"] = GreetingService::formatGreeting(name);
    auto resp = HttpResponse::newHttpJsonResponse(body);
    resp->setStatusCode(code);
    return resp;
}

}  // namespace

void ApiController::getGreeting(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    const std::string param = req->getParameter("name");
    const std::string name = param.empty() ? "World" : param;

    try
    {
        callback(makeGreetingResponse(name, k200OK));
    }
    catch (const std::invalid_argument &e)
    {
        callback(makeErrorResponse(k400BadRequest, e.what()));
    }
}

void ApiController::postGreeting(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) const
{
    const auto jsonPtr = req->getJsonObject();
    if (!jsonPtr)
    {
        callback(makeErrorResponse(k400BadRequest,
                                    "request body must be valid JSON"));
        return;
    }
    if (!jsonPtr->isMember("name") || !(*jsonPtr)["name"].isString())
    {
        callback(makeErrorResponse(
            k400BadRequest,
            R"(JSON body must contain a string field "name")"));
        return;
    }

    const std::string name = (*jsonPtr)["name"].asString();
    try
    {
        callback(makeGreetingResponse(name, k201Created));
    }
    catch (const std::invalid_argument &e)
    {
        callback(makeErrorResponse(k400BadRequest, e.what()));
    }
}

}  // namespace api
