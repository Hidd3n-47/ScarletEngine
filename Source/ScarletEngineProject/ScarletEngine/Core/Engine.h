#pragma once

#include <functional>

#include <ScarletCore/WeakHandle.h>

namespace Scarlet
{

namespace Resource
{
class Mesh;
class Texture;
class ILazyLoadAsset;
} // Namespace Resource.

class Event;
class Window;
class Renderer;
class AssetManager;

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

    inline void ReloadGameDll() const { if (mReloadDllFunction) mReloadDllFunction(); }
    inline void SetReloadDllFunction(const std::function<void()>& reloadDll) { mReloadDllFunction = reloadDll; }

    [[nodiscard]] inline WeakHandle<Window> GetMainWindow() const { return mMainWindow; }

#ifdef DEV_CONFIGURATION
    inline void SetBeginRenderEvent(const std::function<void()>& beginRenderEvent) { mBeginRenderEvent = beginRenderEvent; }
    inline void SetEndRenderEvent(const std::function<void()>& endRenderEvent)     { mEndRenderEvent = endRenderEvent; }
#endif // DEV_CONFIGURATION.
private:
    Engine()  = default;
    ~Engine() = default;

    inline static Engine* mInstance = nullptr;

    WeakHandle<Window> mMainWindow;

    AssetManager* mAssetManager;

    WeakHandle<Resource::ILazyLoadAsset> mUvMapTexture;
    WeakHandle<Resource::ILazyLoadAsset> mGlockTexture;

    WeakHandle<Resource::ILazyLoadAsset> mCube;
    WeakHandle<Resource::ILazyLoadAsset> mMonkey;
    WeakHandle<Resource::ILazyLoadAsset> mCone;
    WeakHandle<Resource::ILazyLoadAsset> mGlock;

    bool mRunning = false;

    std::function<void()> mReloadDllFunction;
    std::function<void()> mBeginRenderEvent;

#ifdef DEV_CONFIGURATION
    std::function<void()> mEndRenderEvent;
#endif // DEV_CONFIGURATION.

    void OnEvent(Event& event);
};

} // Namespace Scarlet.
