#include "./Vulkan.h"


#include "../Debug/Timer.h"
#include "../Logging/Log.h"
#include "Renderer/RenderPass.h"
#include "Surface/Surface.h"

void Plexity::VulkanApplication::run()
{
	initVulkan();

	mainLoop();
	cleanup();
}

void Plexity::VulkanApplication::mainLoop()
{
	PX_TRACE("Starting main loop.");

	// Main application loop, as long as the window should stay open
	while (!glfwWindowShouldClose(window)) {
		// Poll glfw for events, keep the window alive
		glfwPollEvents();
	}
}

void Plexity::VulkanApplication::initVulkan()
{
	Timer initTimer = Timer::startTimer("Vulkan initialization");
	
	PX_INFO("Initializing vulkan application.");

	// Initialize GLFW, used for creating windows
	glfwInit();

	// Tell glfw to not use OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Remove window resizing due to need of recreating the whole graphics pipeline
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create the window
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	// Create a vulkan instance and initialize the debugger
	instance.createInstance();
	debugger.createVulkanDebugger(&instance);

	// Create a window vulkan surface
	surface = Surface::createSurface(instance, window);
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

	renderPass = RenderPass::createRenderPass(&logicalDevice, &swapChain);
	PX_TRACE("Created a render pass, initializing the graphics pipeline.");
	
	pipeline = Pipeline::createGraphicsPipeline(&logicalDevice, &swapChain, &renderPass);
	PX_INFO("Created the graphics pipeline.");

	initTimer.stopTimer(true);
}

void Plexity::VulkanApplication::cleanup() {
	Timer deinitTimer = Timer::startTimer("Vulkan cleanup");
	PX_INFO("Cleaning up, time to wip the floor.");

	// Drop the vulkan debugger
	debugger.destroyVulkanDebugger(&instance);

	pipeline.destroyGraphicsPipeline();
	
	renderPass.destroyRenderPass();
	
	imageViews.destroyImageViews();
	
	swapChain.destroySwapChain();
	
	surface.destroySurface();

	logicalDevice.destroyLogicalDevice();
	
	// Destroy the vulkan instance
	instance.cleanup();

	// Drop glfw window
	glfwDestroyWindow(window);
	glfwTerminate();

	deinitTimer.stopTimer(true);
}