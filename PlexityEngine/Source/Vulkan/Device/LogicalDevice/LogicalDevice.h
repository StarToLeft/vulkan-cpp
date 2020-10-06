#pragma once

#include "../Imports.h"
#include "../../Queues/Queue.h"

namespace Plexity
{
	class Surface;

	class LogicalDevice
	{
	public:
		static LogicalDevice createLogicalDevice(VkPhysicalDevice physicalDevice, Surface* surface);
		void destroyLogicalDevice();

		VkDevice* getDevice() { return &device; }
		Queue* getPresentQueue() { return &presentQueue; }
		
	protected:
		VkDevice device;

		Queue presentQueue;
	};
}

