// End-to-end tests: these boot the real Drogon app (the same app_core the
// production binary links) on a loopback test port and drive it over real
// HTTP, using Drogon's own async HttpClient. This is deliberately a
// different shape of test from test_greeting_service.cpp -- it exercises
// routing, JSON parsing, and status codes, not just the pure logic.

#include "../src/controllers/ApiController.h"  // pulls in app_core's registration

#include <drogon/HttpClient.h>
#include <drogon/drogon.h>
#include <gtest/gtest.h>

#include <future>
#include <string>
#include <thread>

using namespace drogon;

namespace
{

constexpr uint16_t kTestPort = 18099;

struct HttpResult
{
    int statusCode = -1;
    std::string body;
};

// Wraps Drogon's callback-based HttpClient in a blocking call, since GTest
// test bodies run synchronously. This is the standard pattern for testing
// async network code: block the calling thread on a promise/future that the
// async callback fulfills.
HttpResult sendSync(HttpMethod method,
                     const std::string &path,
                     const std::string &jsonBody = "")
{
    auto client =
        HttpClient::newHttpClient("http://127.0.0.1:" +
                                   std::to_string(kTestPort));
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(method);
    req->setPath(path);
    if (!jsonBody.empty())
    {
        req->setBody(jsonBody);
        req->setContentTypeCode(CT_APPLICATION_JSON);
    }

    std::promise<HttpResult> promise;
    auto future = promise.get_future();

    // `client` is captured by value so the shared_ptr keeps the HttpClient
    // alive until the callback fires, even though the local `client`
    // variable above goes out of scope first.
    client->sendRequest(
        req,
        [&promise, client](ReqResult result, const HttpResponsePtr &resp) {
            if (result != ReqResult::Ok || !resp)
            {
                promise.set_value({-1, ""});
                return;
            }
            promise.set_value(
                {static_cast<int>(resp->statusCode()), std::string(resp->body())});
        });

    return future.get();
}

}  // namespace

TEST(ApiIntegrationTest, GetGreetingDefaultsToWorld)
{
    const auto result = sendSync(Get, "/api/greeting");
    EXPECT_EQ(result.statusCode, 200);
    EXPECT_NE(result.body.find("Hello, World!"), std::string::npos);
}

TEST(ApiIntegrationTest, GetGreetingWithNameParam)
{
    const auto result = sendSync(Get, "/api/greeting?name=Ada");
    EXPECT_EQ(result.statusCode, 200);
    EXPECT_NE(result.body.find("Hello, Ada!"), std::string::npos);
}

TEST(ApiIntegrationTest, GetGreetingRejectsWhitespaceOnlyNameParam)
{
    // Note: pass a *raw* space here, not a hand-encoded "%20". Drogon's
    // HttpRequest::setPath() URL-encodes whatever string you give it
    // (including re-escaping any literal '%' it finds), so pre-encoding it
    // ourselves would double-encode and the server would see the literal
    // text "%20%20" instead of two real spaces.
    const auto result = sendSync(Get, "/api/greeting?name=  ");
    EXPECT_EQ(result.statusCode, 400);
}

TEST(ApiIntegrationTest, PostGreetingCreatesGreeting)
{
    const auto result = sendSync(Post, "/api/greeting", R"({"name": "Grace"})");
    EXPECT_EQ(result.statusCode, 201);
    EXPECT_NE(result.body.find("Hello, Grace!"), std::string::npos);
}

TEST(ApiIntegrationTest, PostGreetingRejectsMissingNameField)
{
    const auto result = sendSync(Post, "/api/greeting", R"({})");
    EXPECT_EQ(result.statusCode, 400);
}

TEST(ApiIntegrationTest, PostGreetingRejectsNonStringNameField)
{
    const auto result = sendSync(Post, "/api/greeting", R"({"name": 42})");
    EXPECT_EQ(result.statusCode, 400);
}

TEST(ApiIntegrationTest, PostGreetingRejectsMalformedJson)
{
    const auto result = sendSync(Post, "/api/greeting", "not json");
    EXPECT_EQ(result.statusCode, 400);
}

// Custom main (no gtest_main here): start Drogon's event loop on a
// background thread before the tests run, and cleanly shut it down after --
// mirrors the pattern Drogon's own test suite uses.
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    std::promise<void> serverStarted;
    auto serverStartedFuture = serverStarted.get_future();

    std::thread serverThread([&serverStarted]() {
        app().addListener("127.0.0.1", kTestPort).setThreadNum(1);
        app().getLoop()->queueInLoop([&serverStarted]() {
            serverStarted.set_value();
        });
        app().run();
    });

    serverStartedFuture.wait();

    const int result = RUN_ALL_TESTS();

    app().getLoop()->queueInLoop([]() { app().quit(); });
    serverThread.join();

    return result;
}
