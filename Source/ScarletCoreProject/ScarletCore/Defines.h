#pragma once

#ifdef DEV_CONFIGURATION

#define DEBUG(X) X

#include "ScarletLogger/Log.h"

#define SCARLET_INFO(...)  Scarlet::Log::Info ("Scarlet", __VA_ARGS__)
#define SCARLET_DEBUG(...) Scarlet::Log::Debug("Scarlet", __VA_ARGS__)
#define SCARLET_WARN(...)  Scarlet::Log::Warn ("Scarlet", __VA_ARGS__)
#define SCARLET_ERROR(...) Scarlet::Log::Error("Scarlet", __VA_ARGS__)

#else // DEV_CONFIGURATION.

#define DEBUG(X)

#define SCARLET_INFO(X)
#define SCARLET_DEBUG(X)
#define SCARLET_WARN(X)
#define SCARLET_ERROR(X)

#endif // ! DEV_CONFIGURATION.