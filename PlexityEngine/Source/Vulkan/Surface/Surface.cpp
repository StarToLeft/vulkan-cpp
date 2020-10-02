#include "Surface.h"

#include <libloaderapi.h>
#include <stdexcept>

Plexity::Surface Plexity::Surface::createSurface(Instance& instance, GLFWwindow* window)
{
	VkSurfaceKHR vulkanSurface;
	if (glfwCreateWindowSurface(instance.getInstance(), window, nullptr, &vulkanSurface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
	
	Surface surface;
	surface.surface = vulkanSurface;
	surface.instance = &instance;
	
	return surface;
}

void Plexity::Surface::destroySurface()
{
	vkDestroySurfaceKHR(instance->getInstance(), surface, nullptr);
}
