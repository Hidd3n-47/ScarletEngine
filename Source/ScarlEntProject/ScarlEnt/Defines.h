#pragma once

#ifdef SCARLENT
#define SCARLENT_API __declspec(dllexport)
#else  // SCARLENT.
#define SCARLENT_API __declspec(dllimport)
#endif // !SCARLENT.
