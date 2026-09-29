#pragma once

#include <cstddef>
#include <string>

namespace api
{

// Pure, framework-agnostic logic for building a greeting. Deliberately kept
// free of any Drogon/HTTP types so it can be unit tested in isolation, with
// no server, no event loop, and no network involved.
class GreetingService
{
  public:
    static constexpr std::size_t kMaxNameLength = 100;

    // Builds "Hello, <name>!" from `rawName`, after trimming leading and
    // trailing whitespace.
    //
    // Throws std::invalid_argument if, after trimming, the name is empty or
    // longer than kMaxNameLength characters.
    static std::string formatGreeting(const std::string &rawName);

  private:
    static std::string trim(const std::string &s);
};

}  // namespace api
