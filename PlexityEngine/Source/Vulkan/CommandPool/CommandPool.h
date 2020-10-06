#pragma once
#include "../Device/LogicalDevice/LogicalDevice.h"
#include "../Device/PhysicalDevice/PhysicalDevice.h"

namespace Plexity
{
	class CommandPool
	{
	public:
		static CommandPool createCommandPool(LogicalDevice* device, PhysicalDevice* physicalDevice, Surface* surface);

	public:
		VkCommandPool* getCommandPool() { return &commandPool; }

		void destroyCommandPool() const;
		
	protected:
		VkCommandPool commandPool;
		
		LogicalDevice* device;
	};
}

