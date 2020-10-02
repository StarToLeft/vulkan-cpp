#pragma once
#include <chrono>
#include <string>


#include "../Imports.h"

namespace Plexity
{
	class Timer
	{
	public:
		static Timer startTimer(std::string name);

	public:
		void stopTimer(bool format);

		std::string getName() const { return this->name; }
		
	protected:
		std::string name;
		
		std::chrono::high_resolution_clock::time_point startPoint;
		std::chrono::high_resolution_clock::time_point endPoint;
	};
}

