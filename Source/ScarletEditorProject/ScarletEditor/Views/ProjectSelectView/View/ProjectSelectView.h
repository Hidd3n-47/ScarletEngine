#pragma once

#ifdef DEV_CONFIGURATION

#include "Views/IView.h"

namespace Scarlet::Editor
{

class Panel;

/**
 * @class ProjectSelectView: The view used to create new- and view recent projects.
 */
class ProjectSelectView final : public IView
{
public:
    ProjectSelectView();
    ~ProjectSelectView() override = default;

    ProjectSelectView(const ProjectSelectView&)               = delete;
    ProjectSelectView(ProjectSelectView&&)                    = delete;
    ProjectSelectView& operator=(ProjectSelectView&&)         = delete;
    ProjectSelectView& operator=(const ProjectSelectView&)    = delete;

};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.