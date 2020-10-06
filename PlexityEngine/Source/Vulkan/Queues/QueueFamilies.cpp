#include "QueueFamilies.h"

#include <vector>

Plexity::QueueFamilyIndices Plexity::QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice device, Surface* surface) {
	QueueFamilyIndices indices;

	// Logic to find queue family indices to populate struct with
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Search for a graphics queue family
	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		// Check for a present queue
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface->getSurface(), &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}
	
    return indices;
}
