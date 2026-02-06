#include "TestRegistry.h"

#include <ScarletLogger/log.h>

namespace Scarlet
{

void TestRegistry::Init()
{
    Log::Init();
    InitTestSession();

    RegisterTests();
}

void TestRegistry::RunTests()
{
    Log::Trace(mTestSessionName.c_str(), "Total number of categories for test session \"{}\": {}", mTestSessionName, mTests.size());
    Log::Trace(mTestSessionName.c_str(), "| ==========================================================================================");

    uint32_t totalTestsRun = 0;
    for (const auto& [categoryName, tests] : mTests)
    {
        Log::Trace(mTestSessionName.c_str(), "|  -------------------- \"{}\" ----------------------", categoryName);

        uint32_t passed = 0;
        uint32_t failed = 0;

        for (size_t i = 0; i < tests.size(); ++i)
        {
            ++totalTestsRun;
            const Test& test = tests[i];

            if (test.Run())
            {
                Log::Info(mTestSessionName.c_str(), "| {} / {} | Test Passed: \"{}\"", i + 1, tests.size(), test.GetTestName());

                ++passed;
            }
            else
            {
                Log::Error(mTestSessionName.c_str(), "| {} / {} | Test FAILED: \"{}\"", i + 1, tests.size(), test.GetTestName());

                ++failed;
            }
        }

        Log::Trace(mTestSessionName.c_str(), "| ----------------------------------------------", categoryName);
        Log::Trace(mTestSessionName.c_str(), "| \tTests run   : {}", tests.size());
        Log::Trace(mTestSessionName.c_str(), "| \tTests passed: {}", passed);
        Log::Trace(mTestSessionName.c_str(), "| \tTests failed: {}", failed);

        mPassedTests += passed;
        mFailedTests += failed;
    }

    Log::Trace(mTestSessionName.c_str(), "| ==========================================================================================\n");

    Log::Debug(mTestSessionName.c_str(), "> Tests run   : {}", totalTestsRun);
    Log::Info (mTestSessionName.c_str(), "> Tests passed: {}", mPassedTests);
    // Print the text in red if there were any failed tests.
    if(mFailedTests > 0)
    {
        Log::Error(mTestSessionName.c_str(), "> Tests failed: {}", mFailedTests);
    }
    else
    {
        Log::Info (mTestSessionName.c_str(), "> Tests failed: {}", mFailedTests);
    }
}

} // Scarlet.
