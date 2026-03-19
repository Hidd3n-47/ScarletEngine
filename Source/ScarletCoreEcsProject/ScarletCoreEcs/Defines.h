#pragma once

#ifdef COMPONENTS_BUILD
#define COMPONENT_API __declspec(dllexport)
#else // COMPONENTS_BUILD
#define COMPONENT_API __declspec(dllimport)
#endif // !COMPONENTS_BUILD.
