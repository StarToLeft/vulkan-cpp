#pragma once
#include "../Imports.h"
#include "CommandBuffers/CommandBuffers.h"
#include "CommandPool/CommandPool.h"
#include "Instance/Instance.h"
#include "Surface/Surface.h"
#include "Validation/Debug/Debug.h"

#include "Device/LogicalDevice/LogicalDevice.h"
#include "Device/PhysicalDevice/PhysicalDevice.h"
#include "Images/ImageViews.h"
#include "Pipeline/Pipeline.h"
#include "Renderer/RenderPass.h"
#include "Swapchain/Swapchain.h"
#include "Pipeline/Pipeline.h"
#include "Framebuffers/Framebuffers.h"
#include "Renderer/Renderer.h"

namespace Plexity
{
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	
	class VulkanApplication
	{
	public:
		void run();

		Renderer* getRenderer() { return &renderer; }
		
	private:
		void initVulkan();

		void mainLoop();
		void draw();

		void cleanupSwapChain();
		void recreateSwapChain();

		void cleanup();

	protected:
		std::optional<GLFWwindow*> window;
		bool readyToRender;

		Instance instance = {};
		VulkanDebugLayer debugger = {};

		PhysicalDevice physicalDevice = {};
		LogicalDevice logicalDevice = {};
		
		Surface surface = {};

		SwapChain swapChain = {};

		ImageViews imageViews = {};
		
		RenderPass renderPass;
		Pipeline pipeline;

		Framebuffers framebuffers;

		CommandPool commandPool;
		
		CommandBuffers commandBuffers;

		Renderer renderer;
	};
}
