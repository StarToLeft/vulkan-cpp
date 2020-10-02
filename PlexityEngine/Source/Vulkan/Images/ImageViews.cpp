#include "ImageViews.h"

#include "../Swapchain/Swapchain.h"

Plexity::ImageViews Plexity::ImageViews::createImageViews(SwapChain* swapChain)
{
	ImageViews imageViews;
	imageViews.swapChain = swapChain;
	imageViews.swapChainImages = swapChain->getSwapChainImages();
	imageViews.swapChainImageViews.resize(imageViews.swapChainImages.size());

	for (size_t i = 0; i < imageViews.swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = imageViews.swapChainImages[i];

		// Assign a view mode, in this case 2D
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = imageViews.swapChain->getSwapChainImageFormat();

		// Assign how RGBa (colors) should be handled
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		// Define image as "color targets"
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		// Create image view
		if (vkCreateImageView(imageViews.swapChain->getLogicalDevice()->getDevice(), &createInfo, nullptr, &imageViews.swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}

	return imageViews;
}

void Plexity::ImageViews::destroyImageViews()
{
	// Destroy all image views
	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(swapChain->getLogicalDevice()->getDevice(), imageView, nullptr);
	}
}
