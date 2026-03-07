#pragma once

namespace Scarlet::Editor
{

/**
 * @struct UiControlProperties: A struct to represent the properties of a UI Control.
 */
struct UiControlProperties
{
    // Property Details.
    std::string propertyName = "Default";
    std::string propertyId   = "Default";

    // Control Variables.
    float dragSpeed = 0.05f;

    // Text Formatting.
    bool firstLetterUppercase = true;
};

} // Namespace Scarlet::Editor.
