#include "Swapchain.h"

#include "../Vulkan.h"
#include "../Queues/QueueFamilies.h"

Plexity::SwapChainSupportDetails Plexity::SwapChainSupportDetails::querySwapChainSupport(const VkPhysicalDevice* device, Surface* surface)
{
	SwapChainSupportDetails details;

	// Get surface capabilities, only returns capabilities supported by the created surface
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, surface->getSurface(), &details.capabilities);

	// Query for surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(*device, surface->getSurface(), &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(*device, surface->getSurface(), &formatCount, details.formats.data());
	}

	// Query for supported presentation modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(*device, surface->getSurface(), &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(*device, surface->getSurface(), &presentModeCount, details.presentModes.data());
	}
	
	return details;
}

// Choose the color format and the color space
VkSurfaceFormatKHR Plexity::SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

// Choose a presentation mode
VkPresentModeKHR Plexity::SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	// Try to enable triple buffering
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	// If it's not available; default to FIFO
	return VK_PRESENT_MODE_FIFO_KHR;
}

// Resolution of images in the swap chain
VkExtent2D Plexity::SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { WIDTH, HEIGHT };

		actualExtent.width = std::max<uint32_t>(capabilities.minImageExtent.width, std::min<uint32_t>(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max<uint32_t>(capabilities.minImageExtent.height, std::min<uint32_t>(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

Plexity::SwapChain Plexity::SwapChain::createSwapChain(PhysicalDevice* physicalDevice, LogicalDevice* logicalDevice, Surface* surface, std::optional<VkSwapchainKHR*> oldSwapchain)
{
	VkPhysicalDevice vkPhysicalDevice = physicalDevice->getDevice();

	// Get important information and create needed structures
	SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::querySwapChainSupport(&vkPhysicalDevice, surface);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	// Assign the amount of images to have in swapchain, we will limit it to the (minimum amount + 1) defined by the physical device
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	// Check that the new imageCount doesn't exceed the maximum amount allowed
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Creation information for Vulkan to create the swapchain
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface->getSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;

	// We render directly to the swapchain image, however we could later on render to a different image and copy it to this one
	// In that case use the VK_IMAGE_USAGE_TRANSFER_DST_BIT bit
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(physicalDevice->getDevice(), surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// Apply transform to the swapchain image, like rotating the view
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

	// Block the window from blending with other windows on the alpha value
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;

	// Discard pixels we can't see
	createInfo.clipped = VK_TRUE;

	// Assign the old swapchain, if it existed
	createInfo.oldSwapchain = nullptr;
	if (oldSwapchain.has_value())
	{
		createInfo.oldSwapchain = *oldSwapchain.value();
	}

	// Create the swapchain
	SwapChain swapChain = {};
	swapChain.logicalDevice = logicalDevice;
	if (vkCreateSwapchainKHR(*logicalDevice->getDevice(), &createInfo, nullptr, &swapChain.swapChain) != VK_SUCCESS) {
		throw std::runtime_error("An error occured while creating the swapchain.");
	}

	// Store the swapchain extent and format
	swapChain.swapChainImageFormat = surfaceFormat.format;
	swapChain.swapChainExtent = extent;

	// Set image count
	swapChain.imageCount = imageCount;
	
	return swapChain;
}

void Plexity::SwapChain::destroySwapChain()
{
	// Destroy the swapchain
	vkDestroySwapchainKHR(*logicalDevice->getDevice(), swapChain, nullptr);
}

std::vector<VkImage> Plexity::SwapChain::getSwapChainImages()
{
	std::vector<VkImage> swapChainImages;
	
	// Get the swapchain images
	vkGetSwapchainImagesKHR(*logicalDevice->getDevice(), *getSwapChain(), &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(*logicalDevice->getDevice(), *getSwapChain(), &imageCount, swapChainImages.data());

	return swapChainImages;
}
