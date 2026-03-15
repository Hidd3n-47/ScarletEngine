#pragma once

namespace ScarlEnt
{

/**
 * @enum PropertyType: The C++ type of the property
 */
enum class PropertyType : unsigned char
{
    FLOAT,
    STRING,
    VEC3,
    VEC4,
    QUAT,
    ASSET
};

} // Namespace ScarlEnt.