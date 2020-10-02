#pragma once

#include <string>
#include <vector>


#include "../Imports.h"
#include "../../Instance/Instance.h"
#include "../../Surface/Surface.h"

namespace Plexity
{
	const std::vector<const char*> DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	

	class PhysicalDevice
	{
	public:
		void pickPhysicalDevice(Instance* instance, Surface* surface);

		std::string getName() const { return this->name; }
		VkPhysicalDevice getDevice() const { return this->physicalDevice; }

	private:
		static std::tuple<uint32_t, std::string> rateDeviceSuitability(VkPhysicalDevice device);

	protected:
		VkPhysicalDevice physicalDevice = nullptr;
		std::string name = "";
	};
}
