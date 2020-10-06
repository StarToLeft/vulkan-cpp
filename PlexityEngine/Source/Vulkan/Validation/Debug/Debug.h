#pragma once

#include <../Imports.h>

namespace Plexity
{
	class Instance;

	class VulkanDebugLayer
    {
    public:
        void createVulkanDebugger(Instance* instance);

        void destroyVulkanDebugger(Instance* instance);

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    protected:
        VkDebugUtilsMessengerEXT debugMessenger = {};
    };
}