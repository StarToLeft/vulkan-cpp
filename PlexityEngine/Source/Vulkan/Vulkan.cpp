#include "./Vulkan.h"


#include "../Debug/Timer.h"
#include "../Logging/Log.h"
#include "CommandBuffers/CommandBuffers.h"
#include "CommandPool/CommandPool.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderPass.h"
#include "Surface/Surface.h"

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = static_cast<Plexity::VulkanApplication*>(glfwGetWindowUserPointer(window));
	app->getRenderer()->setFramebufferResized(true);
}

void Plexity::VulkanApplication::run()
{
	// Initialize GLFW, used for creating windows
	glfwInit();

	// Tell glfw to not use OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create the window
	window = std::make_optional(glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr));
	glfwSetWindowUserPointer(window.value(), this);
	glfwSetFramebufferSizeCallback(window.value(), framebufferResizeCallback);

	std::thread initVulkanThread(&VulkanApplication::initVulkan, this);

	mainLoop();

	initVulkanThread.join();

	cleanup();
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
	swapChain = SwapChain::createSwapChain(&physicalDevice, &logicalDevice, &surface, std::nullopt, window.value());
	PX_TRACE("Initialized a new swapchain.");

	// Create image views, "a view into an image"
	imageViews = ImageViews::createImageViews(&swapChain);
	PX_TRACE("Created image views based on swapchain images.");

	// Initialize a renderpass
	renderPass = RenderPass::createRenderPass(&logicalDevice, &swapChain);
	PX_TRACE("Created a render pass, initializing the graphics pipeline.");

	// Create the graphics pipeline
	pipeline = Pipeline::createGraphicsPipeline(&logicalDevice, &swapChain, &renderPass);
	PX_TRACE("Created the graphics pipeline.");

	framebuffers = Framebuffers::createFramebuffers(&imageViews, &renderPass, &swapChain, &logicalDevice);
	PX_TRACE("Created frame-buffers.");

	commandPool = CommandPool::createCommandPool(&logicalDevice, &physicalDevice, &surface);
	PX_TRACE("Created command pool.");

	commandBuffers = CommandBuffers::createCommandBuffers(&logicalDevice, &framebuffers, &commandPool, &renderPass, &swapChain, &pipeline);
	PX_TRACE("Initialized command buffers.");

	renderer = Renderer::createRenderer(&logicalDevice, &swapChain, &commandBuffers, &imageViews);
	PX_INFO("Created the vulkan renderer, ready for rendering.");

	readyToRender = true;

	initTimer.stopTimer(true);
}

void Plexity::VulkanApplication::mainLoop()
{
	PX_TRACE("Starting main loop");

	const bool vsync = false;
	const auto maxFrametime = std::chrono::nanoseconds(69 * 100000);
	
	// Main application loop, as long as the window should stay open
	while (!glfwWindowShouldClose(window.value())) {
		auto startTime = std::chrono::high_resolution_clock::now();
		
		// Poll glfw for events
		glfwPollEvents();

		// Render to the screen
		if (readyToRender)
		{
			draw();
			vkDeviceWaitIdle(*logicalDevice.getDevice());
		}

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

void Plexity::VulkanApplication::draw()
{
	const bool shouldRecreate = renderer.draw();
	if (shouldRecreate)
		recreateSwapChain();
}

void Plexity::VulkanApplication::cleanupSwapChain()
{
	commandBuffers.destroyCommandBuffers();
	commandPool.destroyCommandPool();
	framebuffers.destroyFramebuffers();
	pipeline.destroyGraphicsPipeline();
	renderPass.destroyRenderPass();
	imageViews.destroyImageViews();
	swapChain.destroySwapChain();
}

void Plexity::VulkanApplication::recreateSwapChain()
{
	// Check for minimized window
	int width = 0, height = 0;
	glfwGetFramebufferSize(window.value(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window.value(), &width, &height);
		glfwWaitEvents();
	}
	
	PX_DEBUG("Recreating swap-chain.");
	readyToRender = false;
	
	vkDeviceWaitIdle(*logicalDevice.getDevice());
	
	cleanupSwapChain();

	swapChain = SwapChain::createSwapChain(&physicalDevice, &logicalDevice, &surface, std::nullopt, window.value());
	PX_TRACE("Initialized a new swapchain.");
	
	imageViews = ImageViews::createImageViews(&swapChain);
	PX_TRACE("Created image views based on swapchain images.");

	renderPass = RenderPass::createRenderPass(&logicalDevice, &swapChain);
	PX_TRACE("Created a render pass, initializing the graphics pipeline.");

	pipeline = Pipeline::createGraphicsPipeline(&logicalDevice, &swapChain, &renderPass);
	PX_TRACE("Created the graphics pipeline.");

	framebuffers = Framebuffers::createFramebuffers(&imageViews, &renderPass, &swapChain, &logicalDevice);
	PX_TRACE("Created frame-buffers.");

	commandPool = CommandPool::createCommandPool(&logicalDevice, &physicalDevice, &surface);
	PX_TRACE("Created command pool.");

	commandBuffers = CommandBuffers::createCommandBuffers(&logicalDevice, &framebuffers, &commandPool, &renderPass, &swapChain, &pipeline);
	PX_TRACE("Initialized command buffers.");

	readyToRender = true;
}

void Plexity::VulkanApplication::cleanup() {
	Timer deinitTimer = Timer::startTimer("Vulkan cleanup");
	PX_INFO("Shutting down application.");

	glfwDestroyWindow(window.value());
	glfwTerminate();

	debugger.destroyVulkanDebugger(&instance);
	renderer.destroyRenderer();
	cleanupSwapChain();
	surface.destroySurface();
	logicalDevice.destroyLogicalDevice();
	instance.cleanup();

	deinitTimer.stopTimer(true);
}