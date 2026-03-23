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
     * @brief Register the systems needed for the editor - example, viewport camera.
     */
    void RegisterEditorSystems();

    /**
     * @brief Open a given scene at the passed in filepath.
     * @param filepath The absolute filepath of the scene being opened and loaded into the editor.
     */
    void OpenScene(const std::string& filepath);

    /**
     * @brief Save the currently active editor scene to the given filepath.
     * @param filepath The absolute filepath where the editor scene is being saved to.
     */
    void SaveSceneAs(const std::string& filepath);

    /**
     * @brief Save the currently active editor scene to the same path it was loaded from.
     * @remark If the engine default scene is open, the save will preform no operation.
     */
    void SaveCurrentScene();

    /**
     * @brief End render unbinds frame buffers and renders it to the viewport, as well as renders the rest of the editor UI.
     */
    void EndRender() const;

    /**
     * @brief Get a \ref WeakHandle to the game scene.
     * @return A \ref WeakHandle to the game scene.
     */
    [[nodiscard]] inline WeakHandle<ScarlEnt::Scene> GetGameScene() const { return mEditorScene;}

    /**
     * @brief Get the filepath to the currently open scene.
     * @return The filepath of the currently open scene.
     */
    [[nodiscard]] inline std::string GetCurrentScenePath() const { return mCurrentSceneFilepath; }

    /**
     * @brief Open a game project.
     * @param filepath The absolute filepath of the game project. This is the path to the actual .scarlet_project file.
     */
    inline void OpenGameProject(const std::string& filepath) { mProjectPathToOpen = filepath; }

    /**
     * @brief Get the name of the currently open project.
     * @return The name of the currently opened project.
     */
    inline std::string GetProjectName() const { return mProjectName; }

    inline static constexpr Ulid BACK_ICON_ULID      { 1 };
    inline static constexpr Ulid DIRECTORY_ICON_ULID { 2 };
    inline static constexpr Ulid ASSET_ICON_ULID     { 3 };
    inline static constexpr Ulid SCENE_ICON_ULID     { 4 };
    inline static constexpr Ulid COMPONENT_ICON_ULID { 5 };
    inline static constexpr Ulid PLAY_ICON_ULID      { 6 };
    inline static constexpr Ulid STOP_ICON_ULID      { 7 };
private:
    EditorManager();
    ~EditorManager();
    inline static EditorManager* mInstance = nullptr;

    WeakHandle<ScarlEnt::Scene> mEditorScene;
    std::string mCurrentSceneFilepath{};
    std::string mProjectPathToOpen{};
    std::string mProjectName{};

    ScarlEnt::EntityHandle<Component::Transform, Component::Camera, Component::DirectionLight> mCameraEntity;

    IView* mEditorView = nullptr;

    /**
     * @brief A function called at the end of an update to safely clean-up or remove instances referenced in the update function.
     */
    void PostUpdate();

    /**
     * @brief Internal function to open a project. This opens the project set by the public mutator, this function must be called in post update to safely change.
     */
    void OpenGameProject();
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
