#include "./Vulkan.h"


#include "../Debug/Timer.h"
#include "../Logging/Log.h"
#include "Renderer/RenderPass.h"
#include "Surface/Surface.h"

void Plexity::VulkanApplication::run()
{
	// Initialize GLFW, used for creating windows
	glfwInit();

	// Tell glfw to not use OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Remove window resizing due to need of recreating the whole graphics pipeline
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create the window
	window = std::make_optional(glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr));

	std::thread initVulkanThread(&VulkanApplication::initVulkan, this);

	mainLoop();

	initVulkanThread.join();

	cleanup();
}

void Plexity::VulkanApplication::mainLoop()
{
	PX_TRACE("Starting main loop");
	
	bool vsync = true;
	int maxFrametimeMS = 6.9 * 10;
	auto maxFrametime = std::chrono::nanoseconds(69 * 100000);
	
	// Main application loop, as long as the window should stay open
	while (!glfwWindowShouldClose(window.value())) {
		auto startTime = std::chrono::high_resolution_clock::now();
		
		// Poll glfw for events, keep the window alive
		glfwPollEvents();

		auto endTime = std::chrono::high_resolution_clock::now();

		// Sleep for the required time, for VSync
		if (vsync) {
			auto frameTime = endTime - startTime;
			if (frameTime > maxFrametime)
				continue;

			auto difference = maxFrametime - frameTime;
			std::this_thread::sleep_for(difference);
		}
	}
}

void Plexity::VulkanApplication::initVulkan()
{
	Timer initTimer = Timer::startTimer("Vulkan initialization");
	
	PX_INFO("Initializing vulkan application.");

	// Create a vulkan instance and initialize the debugger
	instance.createInstance();
	debugger.createVulkanDebugger(&instance);

	// Create a window vulkan surface
	surface = Surface::createSurface(instance, window.value());
	PX_TRACE("Created window surface.");
	
	// Choose a valid physical device
	physicalDevice.pickPhysicalDevice(&instance, &surface);
	PX_INFO("Found a physical device, defined as: {}", physicalDevice.getName());

	// Create a logical device
	logicalDevice = LogicalDevice::createLogicalDevice(physicalDevice.getDevice(), &surface);
	PX_TRACE("Created a logical device for physical device: {}", physicalDevice.getName());

	// Create the swapchain
	swapChain = SwapChain::createSwapChain(&physicalDevice, &logicalDevice, &surface, std::nullopt);
	PX_TRACE("Initialized a new swapchain.");

	// Create image views, "a view into an image"
	imageViews = ImageViews::createImageViews(&swapChain);
	PX_TRACE("Created image views based on swapchain images.");

	// Initialize a renderpass
	renderPass = RenderPass::createRenderPass(&logicalDevice, &swapChain);
	PX_TRACE("Created a render pass, initializing the graphics pipeline.");
	
	// Create the graphics pipeline
	pipeline = Pipeline::createGraphicsPipeline(&logicalDevice, &swapChain, &renderPass);
	PX_INFO("Created the graphics pipeline.");

	framebuffers = Framebuffers::createFramebuffers(&imageViews, &renderPass, &swapChain, &logicalDevice);

	initTimer.stopTimer(true);
}

void Plexity::VulkanApplication::cleanup() {
	Timer deinitTimer = Timer::startTimer("Vulkan cleanup");
	PX_INFO("Shutting down application.");

	glfwDestroyWindow(window.value());
	glfwTerminate();

	debugger.destroyVulkanDebugger(&instance);
	pipeline.destroyGraphicsPipeline();
	renderPass.destroyRenderPass();
	imageViews.destroyImageViews();
	swapChain.destroySwapChain();
	surface.destroySurface();
	logicalDevice.destroyLogicalDevice();
	instance.cleanup();

	deinitTimer.stopTimer(true);
}