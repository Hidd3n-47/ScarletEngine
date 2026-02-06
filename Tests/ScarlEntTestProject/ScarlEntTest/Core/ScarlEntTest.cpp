#include <ScarletTestTemplate/Src/EntryPoint.h>
#include <ScarletTestTemplate/Core/TestRegistry.h>

#include "ScarlEntTest/Tests/ArchetypeTests.h"

namespace Scarlet
{

class ScarlEntTest final : public TestRegistry
{
public:
    inline void InitTestSession() override
    {
        mTestSessionName = "ScarlEnt Tests";
    }

    inline void RegisterTests() override
    {
        ArchetypeTests(this);
    }

};

TestRegistry* CreateTestEnvironment() // NOLINT(clang-diagnostic-extra-qualification)
{
    return new ScarlEntTest();
}

} // Namespace Scarlett.
