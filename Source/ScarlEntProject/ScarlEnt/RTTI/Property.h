#pragma once

#include <string>
#include <functional>

#include "ScarlEnt/RTTI/PropertyType.h"
#include "ScarlEnt/ComponentId.h"

namespace ScarlEnt
{
/**
 * @class Property: A class used for reflection to represent a property in a component struct. \\n
 * \ref Property will contain function pointers to the get and set for each property to be able to change it at runtime.
 */
class Property
{
public:
    Property() = default;
    inline Property(const PropertyType type, const ComponentId& componentId, const std::function<std::string()>& getFunction, const std::function<void(const std::string_view&)>& setFunction)
        : mType(type)
        , mComponentId(componentId)
        , mGetFunction(getFunction)
        , mSetFunction(setFunction)
    { /* Empty. */ }

    /**
     * @brief Get the value of the property as a string.
     * @return The value of the property as a string.
     */
    [[nodiscard]] inline std::string GetPropertyValue()         const { return mGetFunction(); }
    /**
     * @brief Set the property value from a string. This will convert the string to the correct type and set the value to that.
     * @param value The string the value is going to be set to.
     */
    inline void SetPropertyValue(const std::string_view& value) const { mSetFunction(value); }

    /**
     * @brief Get the \ref PropertyType of the property.
     * @return The type of the property.
     */
    [[nodiscard]] inline PropertyType GetType()          const { return mType; }
    /**
     * @brief Get the \ref ComponentId of the owner of the property.
     * @return The \c ComponentId that the property belongs to.
     */
    [[nodiscard]] inline ComponentId  GetOwnerComponentId() const { return mComponentId; }

    /**
     * @brief Get the \ref PropertyType as a string.
     * @return The type of the property as a string.
     */
    [[nodiscard]] inline std::string GetTypeAsString() const
    {
        // todo try make constexpr
        if (mType == PropertyType::FLOAT)    return "float";
        if (mType == PropertyType::STRING)   return "string";
        if (mType == PropertyType::VEC3)     return "vec3";
        if (mType == PropertyType::VEC4)     return "vec4";
        if (mType == PropertyType::QUAT)     return "quat";
        if (mType == PropertyType::ASSET)    return "asset";

        return "UNKNOWN";
    }

private:
    PropertyType mType;
    ComponentId  mComponentId;
    std::string mPropertyValue;

    std::function<std::string()>                 mGetFunction;
    std::function<void(const std::string_view&)> mSetFunction;
};

} // Namespace ScarlEnt.