#pragma once

#include <functional>

#include <ScarletCore/WeakHandle.h>

namespace Scarlet
{

namespace Resource
{
class Mesh;
class Texture;
} // Namespace Resource.

class Event;
class Window;
class Renderer;

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
    void Destroy() const noexcept;

    void Run() const;

    inline void CloseEngine() { mRunning = false; }

    inline void ReloadGameDll() { if (mReloadDllFunction) mReloadDllFunction(); }
    inline void SetReloadDllFunction(const std::function<void()>& reloadDll) { mReloadDllFunction = reloadDll; }
private:
    Engine()  = default;
    ~Engine() = default;

    inline static Engine* mInstance = nullptr;

    WeakHandle<Window> mMainWindow;

    void OnEvent(Event& e);

    Resource::Texture* mUvMapTexture;
    Resource::Texture* mGlockTexture;

    Resource::Mesh* mCube;
    Resource::Mesh* mMonkey;
    Resource::Mesh* mCone;
    Resource::Mesh* mGlock;

    bool mRunning = false;

    std::function<void()> mReloadDllFunction;
};

} // Namespace Scarlet.
