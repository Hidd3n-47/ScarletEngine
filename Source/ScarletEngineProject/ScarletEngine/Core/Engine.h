#pragma once

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
    void Destroy() noexcept;

    void Run() const;
private:
    Engine()  = default;
    ~Engine() = default;

    inline static Engine* mInstance = nullptr;

    WeakHandle<Window> mMainWindow;

    bool mRunning = false;

    void OnEvent(Event& e);

    Resource::Texture* mUvMapTexture;

    Resource::Mesh* mCube;
    Resource::Mesh* mMonkey;
    Resource::Mesh* mCone;
};

} // Namespace Scarlet.
