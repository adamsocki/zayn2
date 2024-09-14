
#include <vector>
#include "commonlib.h"

#include "math/math.h"

#include "model.h"

#include "memory.h"

#include "dynamic_array.h"

#include "entity.h"

#include "camera.h"

#include "input.h"

#include "renderer/render.h"

#include "../assets/fonts/stb_font_consolas_24_latin1.inl"

#include "casette/casette_system.h"


enum ZaynState
{
    ZAYN_BASE_STATE,
    ZAYN_CARTRIDGE_STATE
};

struct ShaderCollection
{
    ShaderProgramPaths shaderProgramPaths;
};

struct ZaynMemory
{
    GLFWwindow* window;
    ZaynState zaynState = ZAYN_BASE_STATE;

    vec2 windowSize;

    Model model1;
    Model model2;
    Model casette_model;

    Monkey monkey_1;
    EntityHandle monkeyHandle1;
    EntityHandle casette_handle_1;

    InputManager inputManager;
    InputDevice* keyboard;
    InputDevice* mouse;

    MemoryArena permanentMemArena;
    MemoryArena frameMemArena;

    ShaderCollection shaderCollection;

    VkPipeline vkGraphicsPipeline;
    VkShaderModule vkVertShaderModule;
    VkShaderModule vkFragShaderModule;

    uint32_t g_QueueFamily = -1;
    uint32_t vkQueueFamilyCount = -1;
    VkInstance vkInstance;
    VkDebugUtilsMessengerEXT vkDebugMessenger;
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice vkDevice;
    VkPhysicalDeviceFeatures vkDeviceFeatures{};
    VkQueue vkGraphicsQueue;
    VkSurfaceKHR vkSurface;
    VkQueue vkPresentQueue;
    VkSwapchainKHR vkSwapChain;
    std::vector<VkQueueFamilyProperties> vkQueueFamilies;
    VkAllocationCallbacks*   g_Allocator;

    VkSwapchainKHR vkOldSwapChain;

    std::vector<VkImage> vkDepthImages;
    std::vector<VkDeviceMemory> vkDepthImageMemorys;
    std::vector<VkImageView> vkDepthImageViews;
    std::vector<VkImage> vkSwapChainImages;
	// VkCommandPool vkCommandPool = VK_NULL_HANDLE;

    VkPhysicalDeviceMemoryProperties vkMemProperties;

    VkFormat vkSwapChainImageFormat;
    VkExtent2D vkSwapChainExtent;
    std::vector<VkImageView> vkSwapChainImageViews;
    VkPipelineLayout vkPipelineLayout;
    VkPipelineLayout vkPipelineLayout_blank;
    VkRenderPass vkRenderPass;
    std::vector<VkFramebuffer> vkSwapChainFramebuffers;
    VkCommandPool vkCommandPool;
    VkCommandBuffer vkCommandBuffer;
    VkSemaphore vkImageAvailableSemaphore;
    VkSemaphore vkRenderFinishedSemaphore;
    VkFence vkInFlightFence;

    // VkCommandBuffer* vkCommandBuffers;
    std::vector<VkCommandBuffer> vkCommandBuffers;

    std::vector<VkSemaphore> vkImageAvailableSemaphores;
    std::vector<VkSemaphore> vkRenderFinishedSemaphores;
    std::vector<VkFence> vkInFlightFences;
    std::vector<VkFence> vkImagesInFlight;

    uint32_t vkCurrentFrame = 0;

    // Casstte_Lighting
    VkDescriptorSetLayout vkDescriptorSetLayout_lighting_01;
    VkDescriptorPool vkDescriptorPool_lighting_01;
    std::vector<VkDescriptorSet>  vkDescriptorSets_lighting_01;
    


    bool vkFramebufferResized = false;
    bool vkHasIndexBuffer = false;
    VkBuffer vkVertexBuffer;
    VkDeviceMemory vkVertexBufferMemory;
    VkBuffer vkIndexBuffer;
    VkDeviceMemory vkIndexBufferMemory;
    uint32 vkIndexCount;
    VkBuffer vkInstanceBuffer;
    VkDeviceMemory vkInstanceBufferMemory;

    VkDescriptorSetLayout vkDescriptorSetLayout;
    VkDescriptorSetLayout vkDescriptorSetLayout_blank;

    std::vector<VkBuffer> vkUniformBuffers;
    std::vector<VkDeviceMemory> vkUniformBuffersMemory;
    std::vector<void*> vkUniformBuffersMapped;

    std::vector<VkPushConstantRange> vkPushConstantRanges;

    VkDescriptorPool vkDescriptorPool;
    std::vector<VkDescriptorSet> vkDescriptorSets;
    std::vector<VkDescriptorSet> vkDescriptorSets_blank;

    uint32_t vkMipLevels;
    VkImage vkTextureImage;
    VkDeviceMemory vkTextureImageMemory;
    VkImageView vkTextureImageView;
    VkSampler vkTextureSampler;

    VkImage vkDepthImage;
    VkDeviceMemory vkDepthImageMemory;
    VkImageView vkDepthImageView;

    std::vector<Vertex_> vkVertices;
    std::vector<uint32_t> vkIndices;

    RenderComponent monkey_RC;
    RenderComponent sphereRC;
    RenderComponent grid;
    RenderComponent sphereRC_blank;
    RenderComponent lighting_01;

    DynamicArray<RenderComponent> renderComponents;

    VkDescriptorPool descriptorPool_blank;

    int32 currentLevel;

    // IMGUI code

    VkDescriptorPool imGuiDescriptorPool;
    VkRenderPass imGuiRenderPass;
    std::vector<VkCommandBuffer> imGuiCommandBuffers;
    std::vector<VkFramebuffer> imGuiFrameBuffers;
    VkCommandPool imGuiCommandPool;

    int32 VK_MAX_FRAMES_IN_FLIGHT = 3;

    real32 accumTime = 0.0f;
	uint32 accumFrames = 0;

    uint32 vkCurrentImageIndex;
    bool vkIsFrameStarted = false;
    VkCommandBuffer vkNextCommandBuffer;

    Camera camera;

    uint64 systemTime;
    real32 time;
    real32 deltaTime;
    real32 startTime;
        
    uint32 frame;
    real32 fps;

    void *myCasette;
};

struct ZaynPlatform
{
    ZaynMemory zaynMemory;
    bool running;
};


ZaynMemory *Zayn = NULL;

InputManager *Input = NULL;

InputDevice *Keyboard = NULL;