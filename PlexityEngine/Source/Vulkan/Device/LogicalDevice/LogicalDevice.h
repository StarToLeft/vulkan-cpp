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

		VkDevice getDevice() { return this->device; }
		Queue getPresentQueue() { return this->presentQueue; }
		
	protected:
		VkDevice device;

		Queue presentQueue;
	};
}

