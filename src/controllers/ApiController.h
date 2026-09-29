#pragma once

#include <drogon/HttpController.h>

namespace api
{

// GET  /api/greeting            -> {"message": "Hello, World!"}
// GET  /api/greeting?name=Ada   -> {"message": "Hello, Ada!"}
// POST /api/greeting {"name":"Grace"} -> 201 {"message": "Hello, Grace!"}
class ApiController : public drogon::HttpController<ApiController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ApiController::getGreeting, "/api/greeting", drogon::Get);
    ADD_METHOD_TO(ApiController::postGreeting, "/api/greeting", drogon::Post);
    METHOD_LIST_END

    void getGreeting(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;

    void postGreeting(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;
};

}  // namespace api
