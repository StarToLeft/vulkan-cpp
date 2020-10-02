#pragma once
#include "../Shader/Shader.h"
#include "../Swapchain/Swapchain.h"
#include "../Renderer/RenderPass.h"

namespace Plexity
{
	class Pipeline
	{
	public:
		static Pipeline createGraphicsPipeline(LogicalDevice* logicalDevice, SwapChain* swapChain, RenderPass* renderPass);

	public:
		Shader getShader() { return this->getShader(); }

		void destroyGraphicsPipeline();

		VkPipeline getPipeline() { return this->pipeline; }
		
	protected:
		Shader shader;
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;

		LogicalDevice* logicalDevice;
		RenderPass* renderPass;
	};
}

