#pragma once

#include <ScarletCore/unordered_map.h>

#include "RTTI/Property.h"

namespace ScarlEnt
{

/**
 * @class ComponentView: A class used to get a view into a components properties. This class can be used to iterate through properties and get/set them.
 */
class ComponentView
{
public:
    inline explicit ComponentView(ComponentId componentTypeId, const std::function<unordered_map<std::string, Property>* ()>& getPropertyFunction)
        : mComponentId(std::move(componentTypeId)), mGetPropertyFunction(getPropertyFunction)
    { /* Empty. */ }

    /**
     * @brief Get the component ID of the component that the view is of.
     * @return The component ID of the component that the view is of.
     */
    [[nodiscard]] inline const ComponentId& GetComponentId() const { return mComponentId; }

    /**
     * @brief Get the properties of the component that the view is of.
     * @return The properties of the component that the view is of.
     */
    [[nodiscard]] inline unordered_map<std::string, Property>* GetProperties() const { return mGetPropertyFunction(); }
private:
    ComponentId mComponentId;
    std::function<unordered_map<std::string, Property>* ()> mGetPropertyFunction;

};

} // Namespace ScarlEntt.