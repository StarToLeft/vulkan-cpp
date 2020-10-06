#include "Shader.h"

#include <vector>
#include <fstream>

#include "../../Logging/Log.h"

static std::vector<char> ReadFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }

    const size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule Plexity::Shader::createShaderModule(LogicalDevice* logicalDevice, const std::vector<char>& code)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(*logicalDevice->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }

    return shaderModule;
}

Plexity::Shader Plexity::Shader::createShader(LogicalDevice* logicalDevice, const std::string& vertexShader, const std::string& fragmentShader)
{
    Shader shader;
    shader.logicalDevice = logicalDevice;
	
    shader.vertexShaderData = ReadFile(vertexShader);
    shader.fragmentShaderData = ReadFile(fragmentShader);

    shader.vertShaderModule = createShaderModule(logicalDevice, shader.vertexShaderData);
    shader.fragShaderModule = createShaderModule(logicalDevice, shader.fragmentShaderData);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shader.vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shader.fragShaderModule;
    fragShaderStageInfo.pName = "main";
	
    shader.vertShaderStageInfo = vertShaderStageInfo;
    shader.fragShaderStageInfo = fragShaderStageInfo;

    return shader;
}

void Plexity::Shader::destroyShaderModules() const
{
    vkDestroyShaderModule(*logicalDevice->getDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(*logicalDevice->getDevice(), vertShaderModule, nullptr);
}
