#include "PhysicalDevice.h"

#include <map>
#include <set>
#include <stdexcept>
#include <vector>

#include "../../Instance/Instance.h"
#include "../../Queues/QueueFamilies.h"
#include "../../Swapchain/Swapchain.h"

// Check if device supports extensions
bool CheckDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(Plexity::DEVICE_EXTENSIONS.begin(), Plexity::DEVICE_EXTENSIONS.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void Plexity::PhysicalDevice::pickPhysicalDevice(Instance* instance, Surface* surface)
{
	// Get all available physical devices (GPUs)
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, nullptr);

	// If the count is zero, then we are basically lost.
	if (deviceCount == 0) {
		throw std::runtime_error("There are no physical devices available that support Vulkan.");
	}

	// Get all available vulkan devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, devices.data());

	// Ordered map of physical devices based on their score
	std::multimap<uint32_t, std::tuple<VkPhysicalDevice, std::string>> deviceScores{};
	
	for (const auto& device : devices)
	{
		// Check if the physical device supports a graphical queue
		QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, surface);
		const bool extensionsSupported = CheckDeviceExtensionSupport(device);

		// Check if device supports any presentation modes and surface formats, needed for rendering
		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(&device, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		
		// Continue the loop, skip this device if device is not suitable
		if (!indices.isComplete() || !extensionsSupported || !swapChainAdequate)
			continue;

		// Rate the physical device and insert its rating into the ordered map
		std::tuple<uint32_t, std::string> deviceScore = rateDeviceSuitability(device);
		deviceScores.insert(std::make_pair(std::get<uint32_t>(deviceScore), std::tuple(device, std::get<std::string>(deviceScore))));
	}

	// Get the highest scoring physical device
	if (deviceScores.rbegin()->first > 0) {
		physicalDevice = std::get<VkPhysicalDevice>(deviceScores.rbegin()->second);
		name = std::get<std::string>(deviceScores.rbegin()->second);
	}
	else {
		throw std::runtime_error("No supported GPUs are available.");
	}

	// Check if we found a device
	if (physicalDevice == nullptr) {
		throw std::runtime_error("There where no physical devices that matched expectations.");
	}
}

std::tuple<uint32_t, std::string> Plexity::PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device)
{
	uint32_t score = 0;
	
	// Fetches name, type, vulkan version
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// Rate device by type
	switch (deviceProperties.deviceType) {
	case VK_PHYSICAL_DEVICE_TYPE_OTHER:
		score += 1;
		break;

	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		score += 2;
		break;

	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		score += 3;
		break;

	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		score += 4;
		break;
		
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		score += 5;
		break;
		
	default:
		break;
	}

	// Rate device by maximum texture quality
	score += deviceProperties.limits.maxImageDimension2D;

	// Can't render without geometry shaders
	if (!deviceFeatures.geometryShader)
		return std::make_pair(0, "");

	// Give extra point with higher floating point
	if (deviceFeatures.shaderFloat64)
	{
		score += 1;
	}

	// Give extra score if device supports multiple viewports
	if (deviceFeatures.multiViewport)
	{
		score += 1;
	}
	
	return std::make_pair(score, deviceProperties.deviceName);
}
