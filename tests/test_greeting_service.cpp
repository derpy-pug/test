#include "../src/GreetingService.h"

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

using api::GreetingService;

TEST(GreetingServiceTest, FormatsSimpleName)
{
    EXPECT_EQ(GreetingService::formatGreeting("Alice"), "Hello, Alice!");
}

TEST(GreetingServiceTest, PreservesInternalWhitespace)
{
    EXPECT_EQ(GreetingService::formatGreeting("Ada Lovelace"),
              "Hello, Ada Lovelace!");
}

TEST(GreetingServiceTest, TrimsLeadingAndTrailingWhitespace)
{
    EXPECT_EQ(GreetingService::formatGreeting("   Bob\t\n  "), "Hello, Bob!");
}

TEST(GreetingServiceTest, RejectsEmptyName)
{
    EXPECT_THROW(GreetingService::formatGreeting(""), std::invalid_argument);
}

TEST(GreetingServiceTest, RejectsWhitespaceOnlyName)
{
    EXPECT_THROW(GreetingService::formatGreeting("   \t\n  "),
                 std::invalid_argument);
}

TEST(GreetingServiceTest, AcceptsNameExactlyAtMaxLength)
{
    const std::string name(GreetingService::kMaxNameLength, 'a');
    EXPECT_NO_THROW(GreetingService::formatGreeting(name));
}

TEST(GreetingServiceTest, RejectsNameExceedingMaxLength)
{
    const std::string name(GreetingService::kMaxNameLength + 1, 'a');
    EXPECT_THROW(GreetingService::formatGreeting(name),
                 std::invalid_argument);
}

TEST(GreetingServiceTest, ThrownMessageIsInformative)
{
    try
    {
        GreetingService::formatGreeting("");
        FAIL() << "expected std::invalid_argument to be thrown";
    }
    catch (const std::invalid_argument &e)
    {
        EXPECT_STREQ(e.what(), "name must not be empty");
    }
}
