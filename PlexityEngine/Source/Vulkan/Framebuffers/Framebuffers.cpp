#include "Framebuffers.h"

Plexity::Framebuffers Plexity::Framebuffers::createFramebuffers(ImageViews* imageViews, RenderPass* renderPass, SwapChain* swapChain, LogicalDevice* device)
{
    Framebuffers framebuffers;
    framebuffers.swapChainFramebuffers.resize(imageViews->getSwapChainImageViews()->size());
    framebuffers.device = device;

    // Create framebuffers for each image view
    for (size_t i = 0; i < imageViews->getSwapChainImageViews()->size(); i++) {
        VkImageView attachments[] = {
            imageViews->getSwapChainImageViews()->at(i)
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = *renderPass->getRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain->getExtent2D()->width;
        framebufferInfo.height = swapChain->getExtent2D()->height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(*device->getDevice(), &framebufferInfo, nullptr, &framebuffers.swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    
    return framebuffers;
}

void Plexity::Framebuffers::destroyFramebuffers()
{
	for (VkFramebuffer frameBuffer : swapChainFramebuffers)
	{
        vkDestroyFramebuffer(*device->getDevice(), frameBuffer, nullptr);
	}
}
