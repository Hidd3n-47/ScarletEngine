#pragma once

#ifdef DEV_CONFIGURATION

#include "UiControlProperties.h"

namespace ScarlEnt
{
class Property;
} // Namespace ScarlEnt.

namespace Scarlet::Editor
{

struct UiControlButtonProperties;

/**
 * @class UiControl: Static class to render UI controls for particular types.
 */
class UiControl
{
public:
    /**
     * @brief Add a property control for a bool.
     * @param property The property the control is for.
     * @param controlProperties The properties of the control.
     */
    static void RenderBoolPropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties = {});
    /**
     * @brief Add a property control for a string.
     * @param property The property the control is for.
     * @param controlProperties The properties of the control.
     */
    static void RenderStringPropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties = {});
    /**
     * @brief Add a property control for a float.
     * @param property The property the control is for.
     * @param controlProperties The properties of the control.
     */
    static void RenderFloatPropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties = {});
    /**
     * @brief Add a property control for an angle. This is used to allow for more friendly interaction with quaternions.
     * @param property The property the control is for.
     * @param controlProperties The properties of the control.
     */
    static void RenderAngle(const ScarlEnt::Property& property, const UiControlProperties& controlProperties = {});
    /**
     * @brief Add a property control for a vector 3.
     * @param property The property the control is for.
     * @param controlProperties The properties of the control.
     */
    static void RenderVec3PropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties = {});
    /**
     * @brief Add a property control for a vector 4.
     * @param property The property the control is for.
     * @param controlProperties The properties of the control.
     */
    static void RenderVec4PropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties = {});

    static void RenderAssetPropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties = {});
private:
    /**
     * @brief An internal helper function to render a property control for a float.
     * @param label The label of the float (this can be used to give the float an axis, example x, y or z).
     * @param propertyId The ID of the property.
     * @param value The value of the float.
     * @param controlProperties The properties of the control.
     * @param buttonProperties The properties of the button (button rendered to the left of the float normally used to reset value to default).
     */
    static void RenderFloatPropertyControl(const std::string& label, const std::string& propertyId, float& value, const UiControlProperties& controlProperties, const UiControlButtonProperties& buttonProperties);

    /**
     * @brief Render the Property Control in 2 Columns in the form: Name | [property]. This will set up the columns and add the text on the left.
     * @param outLineHeight A reference to a float to get the line height out the function. The function calculates the line height as a part of the two column setup.
     * @param outPropertyId A reference to a string to get the property ID out the function. The function creates the property ID as a part of the two column setup.
     * @param controlProperties The properties of the control
     */
    static void RenderTwoColumnPropertyControl(float& outLineHeight, std::string& outPropertyId, const UiControlProperties& controlProperties);
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.