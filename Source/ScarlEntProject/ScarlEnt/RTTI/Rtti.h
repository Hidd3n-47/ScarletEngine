#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarletCore/unordered_map.h>

#include <ScarlEnt/RTTI/Property.h>

#define SCARLET_COMPONENT(X)                                                         \
public:                                                                              \
inline unordered_map<std::string, ScarlEnt::Property>* GetProperties()               \
    { GenerateProperties(); return &mProperties; }                                   \
private:                                                                             \
unordered_map<std::string, ScarlEnt::Property> mProperties;                          \
void GenerateProperties();                                                           \
public:


#else // DEV_CONFIGURATION.

#define SCARLET_COMPONENT(X)

#endif // Else DEV_CONFIGURATION.