#pragma once

#include "../Imports.h"

namespace Plexity
{
	class Instance
	{
	public:
		void createInstance();

		VkInstance getInstance() const { return this->instance; }

		void cleanup();

	private:
		VkInstance instance = nullptr;
	};
}