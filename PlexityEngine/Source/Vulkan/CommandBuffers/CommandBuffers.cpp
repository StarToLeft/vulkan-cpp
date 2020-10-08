#include "CommandBuffers.h"

#include "../Pipeline/Pipeline.h"

Plexity::CommandBuffers Plexity::CommandBuffers::createCommandBuffers(LogicalDevice* device, Framebuffers* frameBuffers, CommandPool* commandPool, RenderPass* renderPass, SwapChain* swapChain, Pipeline* graphicsPipeline)
{
	CommandBuffers commandBuffers;
	commandBuffers.commandBuffers.resize(frameBuffers->getSwapChainFramebuffers()->size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = *commandPool->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.commandBuffers.size());

	if (vkAllocateCommandBuffers(*device->getDevice(), &allocInfo, commandBuffers.commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffers.commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = *renderPass->getRenderPass();
		renderPassInfo.framebuffer = frameBuffers->getSwapChainFramebuffers()->at(i);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = *swapChain->getExtent2D();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffers.commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffers.commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipeline());

		vkCmdDraw(commandBuffers.commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers.commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers.commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	
	return commandBuffers;
}

void Plexity::CommandBuffers::destroyCommandBuffers()
{
	
}
