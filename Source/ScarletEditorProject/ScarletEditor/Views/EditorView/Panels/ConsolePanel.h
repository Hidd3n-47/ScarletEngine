#pragma once

#include "UI/Panel.h"

namespace Scarlet::Editor
{

/**
 * @class ConsolePanel: The panel used to be a debug console to output logs and messages to the user using the editor.
 */
class ConsolePanel final : public Panel
{
public:
    explicit inline ConsolePanel(IView* view) : Panel{ view, { .title = "Console" } } { /* Empty */ }
    ~ConsolePanel() override = default;

    ConsolePanel(const ConsolePanel&)            = delete;
    ConsolePanel(ConsolePanel&&)                 = delete;
    ConsolePanel& operator=(ConsolePanel&&)      = delete;
    ConsolePanel& operator=(const ConsolePanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;
};

} // Namespace Scarlet::Editor.
