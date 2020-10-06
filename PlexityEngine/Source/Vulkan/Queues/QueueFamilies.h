#pragma once

#include <optional>

#include "../Imports.h"
#include "../Surface/Surface.h"

namespace Plexity
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, Surface* surface);

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
}