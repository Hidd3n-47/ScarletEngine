#pragma once

#include <ScarletCore/WeakHandle.h>

namespace Scarlet
{

class Window;

class Engine
{
public:
    Engine(const Engine&)            = delete;
    Engine(Engine&&)                 = delete;
    Engine& operator=(Engine&&)      = delete;
    Engine& operator=(const Engine&) = delete;

    [[nodiscard]] inline static Engine& Instance() { return *mInstance; }

    static void Init();
    static void Destroy();

    void Run() const;
private:
    Engine()  = default;
    ~Engine() = default;

    inline static Engine* mInstance = nullptr;

    WeakHandle<Window> mMainWindow;

    bool mRunning = false;
};

} // Namespace Scarlet.
