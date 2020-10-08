#pragma once
#include "../Device/LogicalDevice/LogicalDevice.h"
#include "../Framebuffers/Framebuffers.h"
#include "../CommandPool/CommandPool.h"
#include "../Pipeline/Pipeline.h"

namespace Plexity
{
	class CommandBuffers
	{
	public:
		static CommandBuffers createCommandBuffers(LogicalDevice* device, Framebuffers* frameBuffers, CommandPool* commandPool, RenderPass* renderPass, SwapChain* swapChain, Pipeline* graphicsPipeline);

	public:
		std::vector<VkCommandBuffer>* getCommandBuffers() { return &commandBuffers; }
		
		void destroyCommandBuffers();
		
	protected:
		std::vector<VkCommandBuffer> commandBuffers;
	};
}

