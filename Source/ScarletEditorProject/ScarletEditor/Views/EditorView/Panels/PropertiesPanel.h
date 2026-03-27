#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarletCore/unordered_set.h>

#include "UI/Panel.h"

namespace ScarlEnt
{
class IEntityHandle;
} // Namespace ScarlEnt.

namespace Scarlet::Editor
{

/**
 * @class PropertiesPanel: A panel used to show properties about selected instances.
 */
class PropertiesPanel final : public Panel
{
public:
    explicit PropertiesPanel(IView* view);
    ~PropertiesPanel() override = default;

    PropertiesPanel(const PropertiesPanel&)            = delete;
    PropertiesPanel(PropertiesPanel&&)                 = delete;
    PropertiesPanel& operator=(PropertiesPanel&&)      = delete;
    PropertiesPanel& operator=(const PropertiesPanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;
private:
    void RenderRemoveComponentButton(const std::string& componentName, ScarlEnt::IEntityHandle* entity) const;
    static void RenderAddComponentButton();
    static void RenderAddComponentDropdown(ScarlEnt::IEntityHandle* entity);
    static bool RenderRemoveEntityButton(ScarlEnt::IEntityHandle* entity);

    unordered_set<std::string> mUnremovableComponents;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
