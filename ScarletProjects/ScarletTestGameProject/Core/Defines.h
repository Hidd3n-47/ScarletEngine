#pragma once

#ifdef GAME_DLL_BUILD

#define GAME_API __declspec(dllexport)

#else // GAME_DLL_BUILD.

#define GAME_API __declspec(dllimport)

#endif // !GAME_DLL_BUILD.