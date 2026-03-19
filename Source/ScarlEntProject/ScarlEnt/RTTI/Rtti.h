#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarletCore/unordered_map.h>

#include <ScarlEnt/RTTI/Property.h>

#define SCARLET_COMPONENT(X)                                                         \
public:                                                                              \
static std::string ComponentTypeName() { return std::string{ #X }; }                 \
inline unordered_map<std::string, ScarlEnt::Property>* GetProperties()               \
    { GenerateProperties(); return &mProperties; }                                   \
private:                                                                             \
unordered_map<std::string, ScarlEnt::Property> mProperties;                          \
void GenerateProperties();                                                           \
public:


#else // DEV_CONFIGURATION.

#include <string>

#define SCARLET_COMPONENT(X) static std::string ComponentTypeName() { return std::string{ #X }; }

#endif // Else DEV_CONFIGURATION.