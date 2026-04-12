#pragma once

#include <ScarletCore/Ulid.h>

#ifdef DEV_CONFIGURATION

#include <ScarletCore/unordered_map.h>

#include <ScarlEnt/RTTI/Property.h>

#define SCARLET_COMPONENT(X)                                                         \
public:                                                                              \
[[nodiscard]] inline Scarlet::Ulid GetEntityUniqueId() const { return mEntityId; }   \
inline void SetEntityUniqueId(const Scarlet::Ulid id)  { mEntityId = id;   }         \
static std::string ComponentTypeName() { return std::string{ #X }; }                 \
inline unordered_map<std::string, ScarlEnt::Property>* GetProperties()               \
    { GenerateProperties(); return &mProperties; }                                   \
private:                                                                             \
Scarlet::Ulid mEntityId;                                                             \
unordered_map<std::string, ScarlEnt::Property> mProperties;                          \
void GenerateProperties();                                                           \
public:


#else // DEV_CONFIGURATION.

#include <string>

#define SCARLET_COMPONENT(X)                                                         \
public:                                                                              \
[[nodiscard]] inline Scarlet::Ulid GetEntityUniqueId() const { return mEntityId; }   \
inline void SetEntityUniqueId(const Scarlet::Ulid id)  { mEntityId = id;   }         \
static std::string ComponentTypeName() { return std::string{ #X }; }                 \
private:                                                                             \
Scarlet::Ulid mEntityId;                                                             \
public:

#endif // Else DEV_CONFIGURATION.