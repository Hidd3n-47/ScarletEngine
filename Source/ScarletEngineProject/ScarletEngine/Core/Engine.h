#pragma once

#include "ScarletEngine/Core/Defines.h"

#include <functional>

#include <ScarletCore/WeakHandle.h>

namespace ScarlEnt
{
class Scene;
} // Namespace ScarlEnt.

namespace Scarlet
{

class Event;
class Window;
class Renderer;
class AssetManager;

class ENGINE_API Engine
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

    void RegisterEngineSystems(WeakHandle<ScarlEnt::Scene> scene);

    void LoadScarletAssets();

    inline void CloseEngine() { mRunning = false; }

    [[nodiscard]] inline WeakHandle<Window> GetMainWindow() const { return mMainWindow; }

    [[nodiscard]] inline AssetManager& GetAssetManager() const { return *mAssetManager; }

#ifdef DEV_CONFIGURATION
    inline void ReloadGameDll() const { if (mReloadDllFunction) mReloadDllFunction(); }
    inline void ReloadGame   () const { if (mReloadGameFunction) mReloadGameFunction(); }
    inline void SetReloadDllFunction (const std::function<void()>& reloadDll)  { mReloadDllFunction  = reloadDll; }
    inline void SetReloadGameFunction(const std::function<void()>& reloadGame) { mReloadGameFunction = reloadGame; }

    static void* GetImGuiContext();
    inline void SetBeginRenderEvent(const std::function<void()>& beginRenderEvent) { mBeginRenderEvent      = beginRenderEvent; }
    inline void SetEndRenderEvent(const std::function<void()>& endRenderEvent)     { mEndRenderEvent        = endRenderEvent; }
    inline void SetEditorPostUpdateEvent(const std::function<void()>& postUpdate)  { mEditorPostUpdateEvent = postUpdate; }
#endif // DEV_CONFIGURATION.
private:
    Engine()  = default;
    ~Engine() = default;

    static Engine* mInstance;

    WeakHandle<Window> mMainWindow;

    AssetManager* mAssetManager = nullptr;

    bool mRunning = false;

#ifdef DEV_CONFIGURATION
    std::function<void()> mReloadDllFunction;
    std::function<void()> mReloadGameFunction;
    std::function<void()> mBeginRenderEvent;
    std::function<void()> mEndRenderEvent;
    std::function<void()> mEditorPostUpdateEvent;
#endif // DEV_CONFIGURATION.

    void OnEvent(Event& event);

    static void ApplySpherePhysics();
};

} // Namespace Scarlet.
