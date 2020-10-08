#pragma once
#include "../Imports.h";
#include "../Device/LogicalDevice/LogicalDevice.h"
#include "../Swapchain/Swapchain.h"
#include "../CommandBuffers/CommandBuffers.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace Plexity {
	class Renderer
	{
	public:
		static Renderer createRenderer(LogicalDevice* device, SwapChain* swapChain, CommandBuffers* commandBuffers, ImageViews* imageViews);

	public:
		void createSyncObjects();
		
		void destroyRenderer();

		void draw();

	protected:
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame = 0;
		
		LogicalDevice* device;
		SwapChain* swapChain;
		CommandBuffers* commandBuffers;
		ImageViews* imageViews;
	};
}

