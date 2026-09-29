#include "GreetingService.h"

#include <stdexcept>

namespace api
{

std::string GreetingService::trim(const std::string &s)
{
    const auto begin = s.find_first_not_of(" \t\n\r");
    if (begin == std::string::npos)
    {
        return "";
    }
    const auto end = s.find_last_not_of(" \t\n\r");
    return s.substr(begin, end - begin + 1);
}

std::string GreetingService::formatGreeting(const std::string &rawName)
{
    const std::string name = trim(rawName);

    if (name.empty())
    {
        throw std::invalid_argument("name must not be empty");
    }
    if (name.size() > kMaxNameLength)
    {
        throw std::invalid_argument("name exceeds maximum length of " +
                                     std::to_string(kMaxNameLength) +
                                     " characters");
    }

    return "Hello, " + name + "!";
}

}  // namespace api
