#include "Timer.h"

#include "../Logging/Log.h"

Plexity::Timer Plexity::Timer::startTimer(std::string name)
{
	Timer timer;
	timer.startPoint = std::chrono::high_resolution_clock::now();
	timer.name = name;
	
	return timer;
}

void Plexity::Timer::stopTimer(const bool format)
{
	endPoint = std::chrono::high_resolution_clock::now();
	if (!format) return;

	// Log if formatting is enabled
	PX_DEBUG("[{}]: {} seconds.", name, duration_cast<std::chrono::duration<double>>(endPoint - startPoint).count());
}
