#pragma once
#include "../Imports.h"
#include "../Swapchain/Swapchain.h"

namespace Plexity
{
	class RenderPass
	{
	public:
		static RenderPass createRenderPass(LogicalDevice* logicalDevice, SwapChain* swapChain);

	public:
		void destroyRenderPass() const;

		VkRenderPass* getRenderPass() { return &renderPass; }
		
	protected:
		VkRenderPass renderPass;
		
		LogicalDevice* logicalDevice;
	};
}
