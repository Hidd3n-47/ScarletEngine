#pragma once

#include <format>

namespace Scarlet
{

/**
 * @class Log
 * A class used for logging to the debug console.<br/>
 * @note Ensure that it is initialised before use.
 */
class Log
{
public:
    /**
     * @brief Initialize the logger.
     */
    static void Init();

    /**
     * @brief Used to log info to the console. Info is coloured green.
     */
    static void Info(const char* name, const char* msg);
    /**
     * @brief Used to log debug info to the console. Debug info is coloured blue.
     */
    static void Debug(const char* name, const char* msg);
    /**
     * @brief Used to log warnings to the console. Warnings are coloured yellow.
     */
    static void Warn(const char* name, const char* msg);
    /**
     * @brief Used to log errors to the console. Errors are coloured red.
     */
    static void Error(const char* name, const char* msg);

    /**
     * @brief Used to log info to the console. Info is coloured green.
     */
    template <typename... Args>
    static void Info(const char* name, const char* msg, Args&&... args);

    /**
     * @brief Used to log debug info to the console. Debug info is coloured blue.
     */
    template <typename...Args>
    static void Debug(const char* name, const char* msg, Args&&... args);

    /**
     * @brief Used to log warnings to the console. Warnings are coloured yellow.
     */
    template <typename...Args>
    static void Warn(const char* name, const char* msg, Args&&... args);

    /**
     * @brief Used to log errors to the console. Errors are coloured red.
     */
    template <typename...Args>
    static void Error(const char* name, const char* msg, Args&&... args);
};

/* ============================================================================================================================== */

template <typename... Args>
void Log::Info(const char* name, const char* msg, Args&&... args)
{
    std::string formatted = std::vformat(std::string_view(msg), std::make_format_args(args...));
    Info(name, formatted.c_str());
}

template <typename...Args>
void Log::Debug(const char* name, const char* msg, Args&&... args)
{
    std::string formatted = std::vformat(std::string_view(msg), std::make_format_args(args...));
    Debug(name, formatted.c_str());
}

template <typename...Args>
void Log::Warn(const char* name, const char* msg, Args&&... args)
{
    std::string formatted = std::vformat(std::string_view(msg), std::make_format_args(args...));
    Warn(name, formatted.c_str());
}

template <typename...Args>
void Log::Error(const char* name, const char* msg, Args&&... args)
{
    std::string formatted = std::vformat(std::string_view(msg), std::make_format_args(args...));
    Error(name, formatted.c_str());
}

} // Namespace Scarlet.