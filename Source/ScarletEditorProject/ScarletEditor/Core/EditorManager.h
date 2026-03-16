#pragma once

#include <ScarletCore/WeakHandle.h>

#include <ScarlEnt/EntityHandle.h>

#ifdef DEV_CONFIGURATION

namespace ScarlEnt
{
class Scene;
} // Namespace ScarlEnt.

namespace Scarlet::Component
{
struct Camera;
struct Transform;
struct DirectionLight;
} // Namespace Scarlet::Component.

namespace Scarlet::Editor
{

class IView;

/**
 * @class EditorManager: Singleton class that should manage the editor interface. This class will be responsible for setting up dependencies and views.
 */
class EditorManager
{
public:
    [[nodiscard]] inline static EditorManager& Instance() { return *mInstance; }

    /**
     * @brief Initialise the \ref EditorManager by setting up dependencies and creating the singleton instance.
     */
    static void Init();
    /**
     * @brief Terminate dependencies and destroy the instance of the \ref EditorManager.
     */
    static void Destroy();

    /**
     * @brief Open a given scene at the passed in filepath.
     * @param filepath The filepath of the scene being opened and loaded into the editor.
     */
    void OpenScene(const std::string& filepath);

    /**
     * @brief Save the currently active editor scene to the given filepath.
     * @param filepath The filepath where the editor scene is being saved to.
     */
    void SaveScene(const std::string& filepath);

    /**
     * @brief End render unbinds frame buffers and renders it to the viewport, as well as renders the rest of the editor UI.
     */
    void EndRender() const;

    /**
     * @brief Get a \ref WeakHandle to the game scene.
     * @return A \ref WeakHandle to the game scene.
     */
    [[nodiscard]] inline WeakHandle<ScarlEnt::Scene> GetGameScene() const { return mEditorScene;}
private:
    EditorManager();
    ~EditorManager();
    inline static EditorManager* mInstance = nullptr;

    WeakHandle<ScarlEnt::Scene> mEditorScene;

    ScarlEnt::EntityHandle<Component::Transform, Component::Camera, Component::DirectionLight> mCameraEntity;

    IView* mEditorView;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.