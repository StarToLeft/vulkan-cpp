#pragma once
#include "../Imports.h";

namespace Plexity
{
	class Queue
	{
	public:
		static Queue createQueue(VkDevice device, uint32_t index, uint32_t _index);

		VkQueue getQueue() const { return this->queue; }

	protected:
		VkQueue queue = {};
	};
}

