#include "LogicalDevice.h"

#include <set>
#include <stdexcept>

#include "../../Queues/Queue.h"
#include "../../Queues/QueueFamilies.h"
#include "../../Validation/Validation.h"
#include "../PhysicalDevice/PhysicalDevice.h"

Plexity::LogicalDevice Plexity::LogicalDevice::createLogicalDevice(VkPhysicalDevice physicalDevice, Surface* surface)
{
	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(physicalDevice, surface);

	// Specify the type of queues to create
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	// Create all needed queues
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Specify device features
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.geometryShader = VK_TRUE;
	
	// Create logical device
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	
	createInfo.pEnabledFeatures = &deviceFeatures;

	// Enable extensions, ex. swap-chain
	createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
	createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

	// Add validation debugging layers if need be
	if (ENABLE_VALIDATION_LAYERS)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	} else
	{
		createInfo.enabledLayerCount = 0;
	}

	// Create the logical vulkan device
	VkDevice device;
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device))
	{
		throw std::runtime_error("Failed to initialize a logical device.");
	}
	
	LogicalDevice logicalDevice;
	logicalDevice.device = device;
	logicalDevice.physicalDevice = physicalDevice;
	logicalDevice.presentQueue = Queue::createQueue(device, indices.presentFamily.value(), 0);
	logicalDevice.graphicsQueue = Queue::createQueue(device, indices.graphicsFamily.value(), 0);

	return logicalDevice;
}

void Plexity::LogicalDevice::destroyLogicalDevice()
{
	vkDestroyDevice(device, nullptr);
}

VkSampleCountFlagBits Plexity::LogicalDevice::getUsableSampleCount() {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}