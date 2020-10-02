#pragma once

#include <vector>

#include "../Imports.h"
#include "../Swapchain/Swapchain.h"

namespace Plexity
{
	class ImageViews
	{
	public:
		static ImageViews createImageViews(SwapChain* swapChain);

	public:
		void destroyImageViews();
		
	protected:
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

		SwapChain* swapChain;
	};
}
