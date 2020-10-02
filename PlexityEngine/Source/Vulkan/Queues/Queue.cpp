#include "Queue.h"

Plexity::Queue Plexity::Queue::createQueue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t index)
{
	VkQueue vulkanQueue;
	vkGetDeviceQueue(device, queueFamilyIndex, index, &vulkanQueue);

	Queue queue;
	queue.queue = vulkanQueue;

	return queue;
}
