#pragma once
#include "../Imports.h"
#include "../../Logging/Log.h"
#include "../Instance/Instance.h"

namespace Plexity
{
	class Surface
	{
	public:
		static Surface createSurface(Instance& instance, GLFWwindow* window);
		void destroySurface();

		VkSurfaceKHR getSurface() { return this->surface; }
		
	protected:
		VkSurfaceKHR surface = nullptr;
		Instance* instance = nullptr;
	};
}
