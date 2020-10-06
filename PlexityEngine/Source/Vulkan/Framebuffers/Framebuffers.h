#pragma once
#include "../Imports.h"

#include <vector>

#include "../Swapchain/Swapchain.h"
#include "../Images/ImageViews.h"
#include "../Renderer/RenderPass.h"

namespace Plexity {

	class Framebuffers
	{
	public:
		static Framebuffers createFramebuffers(ImageViews* imageViews, RenderPass* renderPass, SwapChain* swapChain, LogicalDevice* device);

	public:
		std::vector<VkFramebuffer>* getSwapChainFramebuffers() { return &swapChainFramebuffers; }

		void destroyFramebuffers();

	protected:
		std::vector<VkFramebuffer> swapChainFramebuffers;
		LogicalDevice* device;
	};
}
