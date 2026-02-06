#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Test.h"

namespace Scarlet
{
/**
 * @class TestRegistry:
 * A class used to register test methods for when running unit tests. \\n
 * In order to have a project for unit tests, link to this and then \\n
 * have a local class that inherits from \c TestRegistry and \\n
 * implement external \c CreateTestEnvironment to return local class. \\n
 */
class TestRegistry
{
public:
    TestRegistry()              = default;
    virtual ~TestRegistry()     = default;

    TestRegistry(const TestRegistry&)               = delete;
    TestRegistry(TestRegistry&&)                    = delete;
    TestRegistry& operator=(TestRegistry&&)         = delete;
    TestRegistry& operator=(const TestRegistry&)    = delete;

    /**
     * @brief Initialize the test environment.
     */
    void Init();
    /**
     * @brief 'Run' all registered tasks.
     */
    void RunTests();

    /**
     * @brief Initialize the test session. \\n
     * Override this method to implement session specific variables such as the session name.
     */
    virtual void InitTestSession()  = 0;
    /**
     * @brief Register the tests for the session.
     */
    virtual void RegisterTests()    = 0;

    /**
     * @brief Add a test case to the test session.
     * @param category The category that the test is a part of.
     * @param testName The name of the specific test.
     * @param test A function pointer to the test.
     */
    inline void AddTestCase(const std::string_view category, const std::string_view testName, const std::function<bool()>& test) { mTests[category].emplace_back(testName, test); }

protected:
    std::string mTestSessionName;

private:
    std::unordered_map<std::string_view, std::vector<Test>> mTests;

    uint32_t mPassedTests = 0;
    uint32_t mFailedTests = 0;
};

TestRegistry* CreateTestEnvironment();

} // Namespace Scarlet.
