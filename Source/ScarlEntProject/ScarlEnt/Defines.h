#pragma once

// Todo Christian: This should be moved into a project definition. Tests should live in a different sln which can define this during compilation.
#define SCARLENT_TEST

#ifdef SCARLENT
#define SCARLENT_API __declspec(dllexport)
#else  // SCARLENT.
#define SCARLENT_API __declspec(dllimport)
#endif // !SCARLENT.

#ifdef DEV_CONFIGURATION

#include <assert.h>

#ifdef SCARLENT_TEST
#include <stdexcept>
#define SCARLENT_ASSERT(...) if (!(__VA_ARGS__)) throw std::runtime_error("Assertion failed!")
#else // SCARLENT_TEST.
#define SCARLENT_ASSERT(...) assert(__VA_ARGS__)
#endif // !SCARLENT_TEST.

#else // DEV_CONFIGURATION.

#define SCARLENT_ASSERT(...)

#endif // ! DEV_CONFIGURATION.