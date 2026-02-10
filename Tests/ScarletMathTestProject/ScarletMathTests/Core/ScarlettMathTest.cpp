#include <ScarletTestTemplate/Src/EntryPoint.h>
#include <ScarletTestTemplate/Core/TestRegistry.h>

#include "ScarletMathTests/Tests/QuaternionTests.h"
#include "ScarletMathTests/Tests/BoundingBoxIntersectionTests.h"

namespace Scarlet
{

class ScarlettMathTest final : public TestRegistry
{
public:
    inline void InitTestSession() override
    {
        mTestSessionName = "ScarletMathTest";
    }

    inline void RegisterTests() override
    {
        QuaternionTests(this);
        BoundingBoxIntersectionTests(this);
    }

};

Scarlet::TestRegistry* Scarlet::CreateTestEnvironment()  // NOLINT(clang-diagnostic-extra-qualification)
{
    return new ScarlettMathTest();
}

} // Namespace Scarlett.
