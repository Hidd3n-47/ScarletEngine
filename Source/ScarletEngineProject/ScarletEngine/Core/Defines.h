#pragma once

#ifdef ENGINE_BUILD
#define ENGINE_API __declspec(dllexport)
#else // ENGINE_BUILD.
#define ENGINE_API __declspec(dllimport)
#endif // !ENGINE_BUILD.
