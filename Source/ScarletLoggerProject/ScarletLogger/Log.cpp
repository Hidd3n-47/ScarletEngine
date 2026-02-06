#include "Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Scarlet
{

void Log::Init()
{
    spdlog::set_level(spdlog::level::trace);

    spdlog::set_pattern("%^[%T]%v%$");
}

void Log::Info(const char* name, const char* msg)
{
    spdlog::info(std::format(" {}: {}", name, msg));
}

void Log::Debug(const char* name, const char* msg)
{
    spdlog::debug(std::format(" {}: {}", name, msg));
}

void Log::Warn(const char* name, const char* msg)
{
    spdlog::warn(std::format(" {}: {}", name, msg));
}

void Log::Error(const char* name, const char* msg)
{
    spdlog::error(std::format(" {}: {}", name, msg));
}

} // Namespace Scarlet.