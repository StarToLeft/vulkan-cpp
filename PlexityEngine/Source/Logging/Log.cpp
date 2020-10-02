#include "Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Plexity
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

	void Log::init()
	{
		//spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v", spdlog::pattern_time_type::local);
		spdlog::stdout_color_mt("Plexity");

		s_CoreLogger = spdlog::get("Plexity");
		s_CoreLogger->set_level(spdlog::level::trace);

		// Set colors
		auto* sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(s_CoreLogger->sinks().back().get());
		sink->set_color(spdlog::level::info, sink->GREEN);
		sink->set_color(spdlog::level::trace, sink->YELLOW);
		sink->set_color(spdlog::level::critical, sink->RED);
		sink->set_color(spdlog::level::warn, sink->YELLOW);
		sink->set_color(spdlog::level::debug , sink->CYAN);
	}
}
