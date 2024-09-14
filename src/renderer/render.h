

#if VULKAN

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};


struct ModelPushConstant
{
    mat4 model_1;
};

    // alignas(16) mat4 model;
struct UniformBufferObject
{
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct UniformBufferObject_lighting_01
{
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
    alignas(16) vec3 lightColor;
};


struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

#endif