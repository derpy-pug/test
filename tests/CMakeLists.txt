# Unit tests: pure logic, no server, no sockets, no event loop. This is the
# bulk of what should run on every commit, since it's fast and deterministic.
add_executable(unit_tests test_greeting_service.cpp)
target_link_libraries(unit_tests PRIVATE app_core GTest::gtest GTest::gtest_main)
add_test(NAME unit_tests COMMAND unit_tests)

# Integration tests: boot the real Drogon app on a test port in-process and
# hit it over real HTTP, exercising routing, JSON (de)serialization, and
# status codes end to end. Note: no gtest_main here -- this binary supplies
# its own main() so it can start/stop the Drogon event loop around the tests.
add_executable(integration_tests test_api_integration.cpp)
target_link_libraries(integration_tests PRIVATE app_core GTest::gtest)
add_test(NAME integration_tests COMMAND integration_tests)
set_tests_properties(integration_tests PROPERTIES TIMEOUT 30)
