#pragma once
#include <vector>

namespace Plexity
{
    static const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    static const bool ENABLE_VALIDATION_LAYERS = false;
#else
    static const bool ENABLE_VALIDATION_LAYERS = true;
#endif

    class VulkanExtensions
    {
    public:
        static bool CheckValidationLayerSupport();

        static std::vector<const char*> GetRequiredExtensions();
    };
}
