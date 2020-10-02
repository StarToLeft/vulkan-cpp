#pragma once

#include <memory>

#include "spdlog/logger.h"

namespace Plexity
{
	class Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}

#define PX_FATAL(...)			::Plexity::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define PX_TRACE(...)			::Plexity::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PX_ERROR(...)			::Plexity::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PX_WARN(...)			::Plexity::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PX_INFO(...)			::Plexity::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PX_DEBUG(...)			::Plexity::Log::GetCoreLogger()->debug(__VA_ARGS__)
