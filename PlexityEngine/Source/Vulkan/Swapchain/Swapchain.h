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
		static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		static SwapChain createSwapChain(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, Surface* surface, std::optional<VkSwapchainKHR*> oldSwapchain);

	public:
		void destroySwapChain();

		std::vector<VkImage> getSwapChainImages();
		
		VkSwapchainKHR getSwapChain() { return this->swapChain; }
		VkFormat getSwapChainImageFormat() { return this->swapChainImageFormat; }
		VkExtent2D getExtent2D() { return this->swapChainExtent; }
		
		LogicalDevice* getLogicalDevice() { return this->logicalDevice; }

	protected:
		VkSwapchainKHR swapChain;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		uint32_t imageCount;
		
		LogicalDevice* logicalDevice;
	};
}

