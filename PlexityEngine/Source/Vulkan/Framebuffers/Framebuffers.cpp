#include "Framebuffers.h"

Plexity::Framebuffers Plexity::Framebuffers::createFramebuffers(ImageViews* imageViews, RenderPass* renderPass, SwapChain* swapChain, LogicalDevice* device)
{
    Framebuffers framebuffer;
    framebuffer.swapChainFramebuffers.resize(imageViews->getSwapChainImageViews()->size());

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

        if (vkCreateFramebuffer(*device->getDevice(), &framebufferInfo, nullptr, &framebuffer.swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    
    return framebuffer;
}

void Plexity::Framebuffers::destroyFramebuffers()
{
}
