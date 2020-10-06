#include "CommandPool.h"

#include "../Queues/QueueFamilies.h"

Plexity::CommandPool Plexity::CommandPool::createCommandPool(LogicalDevice* device, PhysicalDevice* physicalDevice, Surface* surface)
{
	CommandPool commandPool;
	commandPool.device = device;
	
	QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::findQueueFamilies(physicalDevice->getDevice(), surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(*device->getDevice(), &poolInfo, nullptr, &commandPool.commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}

	return commandPool;
}

void Plexity::CommandPool::destroyCommandPool() const
{
	vkDestroyCommandPool(*device->getDevice(), commandPool, nullptr);
}
