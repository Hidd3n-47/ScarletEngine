#pragma once

#include <ScarletCore/WeakHandle.h>

namespace Scarlet
{

class Event;
class Window;

class Engine
{
public:
    Engine(const Engine&)            = delete;
    Engine(Engine&&)                 = delete;
    Engine& operator=(Engine&&)      = delete;
    Engine& operator=(const Engine&) = delete;

    [[nodiscard]] inline static Engine& Instance() noexcept { return *mInstance; }

    static void CreateEngine() noexcept;

    void Init() noexcept;
    void Destroy() noexcept;

    void Run() const;
private:
    Engine()  = default;
    ~Engine() = default;

    inline static Engine* mInstance = nullptr;

    WeakHandle<Window> mMainWindow;

    bool mRunning = false;

    void OnEvent(Event& e);
};

} // Namespace Scarlet.
