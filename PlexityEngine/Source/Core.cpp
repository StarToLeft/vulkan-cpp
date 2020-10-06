#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <cstdlib>
#include <exception>

#include "Logging/Log.h"
#include "Vulkan/Vulkan.h"

using namespace Plexity;

int main() {
    // Initialize the logger
    Log::init();

	// Create the application
    VulkanApplication application;
	
    try
    {
    	// Run it
        application.run();
    } catch (const std::exception& e)
    {
    	// Prevent pc crashes during application errors
        PX_FATAL("Fatal error occured: {}", e.what());
        return EXIT_FAILURE;
    }

	// The program stopped successfully, something finally went right!
    return EXIT_SUCCESS;
}