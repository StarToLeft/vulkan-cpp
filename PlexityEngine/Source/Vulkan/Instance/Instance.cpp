#include "Instance.h"


#include "../Vulkan.h"
#include "../../Logging/Log.h"
#include "../Validation/Validation.h"

namespace Plexity
{
	// Check if all extensions are present
	static void CheckRequiredExtensionsPresent(std::vector<VkExtensionProperties> availableExt, const char** requiredExt, const int requiredExtCount) {
		for (int i = 0; i < requiredExtCount; i++) {
			bool found = false;
			for (const auto& extension : availableExt) {
				if (strcmp(requiredExt[i], extension.extensionName) != 0) {
					found = true;
				}
			}
			if (!found) {
				throw std::runtime_error("Missing vulkan extensions");
			}
		}

		PX_INFO("All extension requirements were fulfilled.");
	}
}

void Plexity::Instance::createInstance()
{
	// Validation layers for debugging
	if (ENABLE_VALIDATION_LAYERS && !VulkanExtensions::CheckValidationLayerSupport()) {
		throw std::runtime_error("Requires validation layers but failed to find any.");
	}

	// Define Vulkan application information
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Triangle Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.pEngineName = "Unity4D";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	// VkInstance creation information
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Debugging validation layers
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

		VulkanDebugLayer::populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}
	

	// Get required extensions
	uint32_t glfwExtensionCount = 0;
	auto* glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	PX_TRACE("Available vulkan extensions:");
	for (auto i = 0; i < glfwExtensionCount; i++) {
		PX_TRACE("\t{}", glfwExtensions[i]);
	}

	// Check if the extensions are available
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> validateExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, validateExtensions.data());
	
	// Check for extension support
	CheckRequiredExtensionsPresent(validateExtensions, glfwExtensions, glfwExtensionCount);

	auto extensions = VulkanExtensions::GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vulkan instance.");
	}
}

void Plexity::Instance::cleanup()
{
	vkDestroyInstance(instance, nullptr);
}
