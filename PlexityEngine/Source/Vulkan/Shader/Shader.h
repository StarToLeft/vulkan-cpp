#pragma once

#include <string>
#include <vector>

#include "../Imports.h"
#include "../Device/LogicalDevice/LogicalDevice.h"

namespace Plexity
{
	class Shader
	{
	public:
		static VkShaderModule createShaderModule(LogicalDevice* logicalDevice, const std::vector<char>& code);
		static Shader createShader(LogicalDevice* logicalDevice, const std::string& vertexShader, const std::string& fragmentShader);

	public:
		void destroyShaderModules() const;

		VkShaderModule getVertShaderModule() { return this->vertShaderModule; }
		VkShaderModule getFragShaderModule() { return this->fragShaderModule; }

		VkPipelineShaderStageCreateInfo* getShaderStages() { return this->shaderStages; }

	protected:
		std::vector<char> vertexShaderData;
		std::vector<char> fragmentShaderData;

		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
		
		VkPipelineShaderStageCreateInfo* shaderStages;
		
		LogicalDevice* logicalDevice;
	};
}

