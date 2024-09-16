

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

struct UniformBufferObject_lighting_02
{
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
    alignas(16) vec3 lightColor;
    alignas(16) vec3 lightPos;
    alignas(16) vec3 viewPos;
};

struct UniformBufferObject_lighting_03
{
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
    alignas(16) vec3 lightColor;
    alignas(16) vec3 lightPos;
    alignas(16) vec3 viewPos;
};

struct MaterialBufferObject_lighting_03
{
    alignas(16) vec3 ambient;
    alignas(16) vec3 diffuse;
    alignas(16) vec3 specular;
    alignas(4) float shininess;
};


struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

#endif