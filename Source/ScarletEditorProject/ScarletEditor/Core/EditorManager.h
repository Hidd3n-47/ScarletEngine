#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarletCore/PrimitiveTypes.h>

namespace Scarlet
{
class Framebuffer;
} // Namespace Scarlet.

namespace Scarlet::Editor
{
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
     * @brief Begin render is used to bind the frame buffer that all render commands will be rendered to for the viewport.
     */
    void BeingRender() const;

    /**
     * @brief End render unbinds frame buffers and renders it to the viewport, as well as renders the rest of the editor UI.
     */
    void EndRender();
private:
    EditorManager();
    ~EditorManager();
    inline static EditorManager* mInstance = nullptr;

    uint32 mLastFrameWidth, mLastFrameHeight;
    uint32 mViewportWidth, mViewportHeight;

    Framebuffer* mFramebuffer;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.