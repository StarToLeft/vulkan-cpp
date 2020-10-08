#pragma once
#include <optional>
#include <vector>

#include "../Imports.h"
#include "../Device/LogicalDevice/LogicalDevice.h"
#include "../Device/PhysicalDevice/PhysicalDevice.h"
#include "../Surface/Surface.h"

namespace Plexity
{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		static SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice* device, Surface* surface);
	};
	
	class SwapChain
	{
	public:
		static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

		static SwapChain createSwapChain(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, Surface* surface, std::optional<VkSwapchainKHR*> oldSwapchain, GLFWwindow* window);

	public:
		void destroySwapChain();

		std::vector<VkImage> getSwapChainImages();
		
		VkSwapchainKHR* getSwapChain() { return &swapChain; }
		VkFormat* getSwapChainImageFormat() { return &swapChainImageFormat; }
		VkExtent2D* getExtent2D() { return &swapChainExtent; }
		
		LogicalDevice* getLogicalDevice() { return logicalDevice; }

	protected:
		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		uint32_t imageCount;
		
		LogicalDevice* logicalDevice;
	};
}

