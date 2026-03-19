#pragma once


#include <string>
#include <stdexcept>

#include <ScarletMath/Quat.h>

#include <ScarletCore/AssetRef.h>

namespace Scarlet
{
/**
 * @class ReflectType: A static class to take a type and convert it to a string, or a string and convert it to a type.
 */
class ReflectType
{
public:
    /**
     * @brief Convert a type to a string.
     * @tparam T The type
     * @param value The value being converted to a string.
     * @return A string that represents the value.
     */
    template <typename T>
    [[nodiscard]] static std::string GetStringFromValue(const T value);

    /**
     * @brief Convert a string to a type.
     * @tparam T The type the string is being converted to.
     * @param value The value that the string will be output to.
     * @param stringValue The string value.
     */
    template <typename T>
    static void SetValueFromString(T& value, const std::string_view& stringValue);
};

/*
  ======================================================================================================================================================
                                                                                                                                                        */

// --------- Gets. ---------
template <typename T>
inline std::string ReflectType::GetStringFromValue(const T value)
{
    throw std::runtime_error("Failed to get reflection value for type: " + std::string{ typeid(T).name() });
}

template <>
inline std::string ReflectType::GetStringFromValue<float>(const float value)
{
    return std::to_string(value);
}

template <>
inline std::string ReflectType::GetStringFromValue<std::string>(const std::string value)
{
    return std::string{ value };
}

template <>
inline std::string ReflectType::GetStringFromValue<Math::Vec3>(const Math::Vec3 value)
{
    return std::to_string(value.x) + "," + std::to_string(value.y) + "," + std::to_string(value.z);
}

template <>
inline std::string ReflectType::GetStringFromValue<Math::Vec4>(const Math::Vec4 value)
{
    return std::to_string(value.x) + "," + std::to_string(value.y) + "," + std::to_string(value.z) + "," + std::to_string(value.w);
}

template <>
inline std::string ReflectType::GetStringFromValue<Math::Quat>(const Math::Quat value)
{
    return GetStringFromValue<Math::Vec4>(value.ToVector4());
}

template <>
inline std::string ReflectType::GetStringFromValue<AssetRef>(const AssetRef value)
{
    return std::to_string(static_cast<int>(value.assetType)) + "," + std::to_string(value.assetUlid);
}

// --------- Sets. ---------
template <typename T>
inline void ReflectType::SetValueFromString(T& value, const std::string_view& stringValue)
{
    throw std::runtime_error("Failed to get reflection value for type: " + std::string{ typeid(T).name() });
}

template <>
inline void ReflectType::SetValueFromString<float>(float& value, const std::string_view& stringValue)
{
    const std::string str = std::string{ stringValue };
    value = std::stof(str);
}

template <>
inline void ReflectType::SetValueFromString<std::string>(std::string& value, const std::string_view& stringValue)
{
    value = std::string{ stringValue };
}

template <>
inline void ReflectType::SetValueFromString<Math::Vec3>(Math::Vec3& value, const std::string_view& stringValue)
{
    const size_t firstCommaPosition  = stringValue.find_first_of(',');
    const size_t secondCommaPosition = stringValue.find(',', firstCommaPosition + 1);

    //todo assert on positions;
    const std::string value1 = std::string{ stringValue.substr(0, firstCommaPosition) };
    const std::string value2 = std::string{ stringValue.substr(firstCommaPosition + 1, secondCommaPosition - firstCommaPosition - 2) };
    const std::string value3 = std::string{ stringValue.substr(secondCommaPosition + 1) };

    value = Math::Vec3(std::stof(value1), std::stof(value2), std::stof(value3));
}

template <>
inline void ReflectType::SetValueFromString<Math::Vec4>(Math::Vec4& value, const std::string_view& stringValue)
{
    const size_t firstCommaPosition  = stringValue.find_first_of(',');
    const size_t secondCommaPosition = stringValue.find(',', firstCommaPosition + 1);
    const size_t thirdCommaPosition  = stringValue.find(',', secondCommaPosition + 1);

    //todo assert on positions;
    const std::string value1 = std::string{ stringValue.substr(0, firstCommaPosition) };
    const std::string value2 = std::string{ stringValue.substr(firstCommaPosition + 1, secondCommaPosition - firstCommaPosition - 2) };
    const std::string value3 = std::string{ stringValue.substr(secondCommaPosition + 1, thirdCommaPosition - secondCommaPosition - 2) };
    const std::string value4 = std::string{ stringValue.substr(thirdCommaPosition + 1) };

    value = Math::Vec4{ std::stof(value1), std::stof(value2), std::stof(value3), std::stof(value4) };
}

template <>
inline void ReflectType::SetValueFromString<Math::Quat>(Math::Quat& value, const std::string_view& stringValue)
{
    Math::Vec4 quatAsVec4{};
    SetValueFromString(quatAsVec4, stringValue);

    value = Math::Quat{ quatAsVec4 };
}

template <>
inline void ReflectType::SetValueFromString<AssetRef>(AssetRef& value, const std::string_view& stringValue)
{
    const size_t firstCommaPosition = stringValue.find_first_of(',');

    value.assetType = static_cast<AssetType>(std::stoi(std::string{ stringValue.substr(0, firstCommaPosition) }));
    value.assetUlid = std::stoull(std::string{ stringValue.substr(firstCommaPosition + 1) });
}

} // Namespace Scarlet.
