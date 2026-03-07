#pragma once

#include "UI/Panel.h"

namespace Scarlet::Editor
{

/**
 * @class PropertiesPanel: A panel used to show properties about selected instances.
 */
class PropertiesPanel final : public Panel
{
public:
    explicit inline PropertiesPanel(IView* view) : Panel{ view, { .title = "Properties" } } { /* Empty */ }
    ~PropertiesPanel() override = default;

    PropertiesPanel(const PropertiesPanel&)            = delete;
    PropertiesPanel(PropertiesPanel&&)                 = delete;
    PropertiesPanel& operator=(PropertiesPanel&&)      = delete;
    PropertiesPanel& operator=(const PropertiesPanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;
};

} // Namespace Scarlet::Editor.
