


#define OPENGL 0
#define VULKAN 1
#define IMGUI 1

#ifdef VULKAN
#define ENGINE_2D 0
#define ENGINE_3D 1
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_glfw.h"
#include "../external/imgui/backends/imgui_impl_vulkan.h"


#define GLFW_INCLUDE_VULKAN
// Volk headers

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
    // #include <unistd.h>
#include <nlohmann/json.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../include/tiny_obj_loader.h"

// #include "../include/stb_truetype.h"s

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <chrono>


#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
// #include <glm/glm.hpp>

#include "data_types.h"

#include "config.h"


#if WINDOWS
#include <cstring>
#endif
#include <GLFW/glfw3.h>

#if _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3native.h>

#endif

// #include <glad/glad.h>




struct SystemPlatform
{
    bool running;
};



void framebuffer_size_callback(GLFWwindow* window, int width, int height);



#include "zayn.cpp"


int main(void)
{

    SystemPlatform platform = {};
    ZaynPlatform zaynPlatform = {};

    ZaynMemory* zaynMemory = &zaynPlatform.zaynMemory;

    zaynPlatform.running = true;

    Zayn = zaynMemory;

    AllocateMemoryArena(&Zayn->permanentMemArena, Megabytes(256));
    AllocateMemoryArena(&Zayn->frameMemArena, Megabytes(32));

    InputManager *inputManager = &zaynMemory->inputManager;


    AllocateInputManager(inputManager, &zaynMemory->permanentMemArena, 4);

    zaynMemory->keyboard = &inputManager->devices[0];
    AllocateInputDevice(zaynMemory->keyboard, InputDeviceType_Keyboard, Input_KeyboardDiscreteCount, 0);

    Keyboard = zaynMemory->keyboard;
    InitializeKeyMap();

    /* Initialize the library */
    if (!glfwInit())
    {

        return -1;
    }

#if OPENGL
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif VULKAN   
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#endif

#ifdef __APPLE__
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


#endif
   /* Create a windowed mode window and its OpenGL context */


//     #ifdef _WIN32
//     #define ROOT_DIR "C:/dev/zayn/"
// #elif __APPLE__
//     #define ROOT_DIR "/Users/socki/dev/zayn/"
// #endif
    Zayn->windowSize.x = 1000;
    Zayn->windowSize.y = 800;

    Zayn->window = glfwCreateWindow(Zayn->windowSize.x, Zayn->windowSize.y, "Hello World", NULL, NULL);
    if (!Zayn->window)
    {
        glfwTerminate();
        return -1;
    }
  

    /* Make the window's context current */
    glfwMakeContextCurrent(Zayn->window);
    glfwSetWindowUserPointer(Zayn->window, Zayn);
    glfwSetFramebufferSizeCallback(Zayn->window, framebuffer_size_callback);


    struct timespec spec;
    double startTime = {};
    
    #ifdef __APPLE__
        clock_gettime(CLOCK_MONOTONIC, &spec);
    startTime = (spec.tv_sec * 1000.0) + (spec.tv_nsec / 1.0e6);
        
    #else
        // Windows appropriate solution
        // Use QueryPerformanceCounter for high-resolution timer
        LARGE_INTEGER frequency;
        LARGE_INTEGER counter;
        if (!QueryPerformanceFrequency(&frequency))
        {
            // Handle error
        }
        if (!QueryPerformanceCounter(&counter))
        {
            // Handle error
        }
        startTime = counter.QuadPart * 1000 / frequency.QuadPart;
    #endif
    double gameTime = 0.0;
    double systemTime = startTime;
    double prevSystemTime = systemTime;
    double deltaTime;

    // seconds
    double frameRate = 1.0 / 6000.0; // Hertz
    double timeSinceRender = 0.0;


    // GLenum err = glewInit();
    ZaynInit(zaynMemory);

   #if IMGUI

    VkDescriptorPoolCreateInfo poolInfo{};
    // ImGui Descriptor pool
    VkDescriptorPoolSize pool_sizes[] = {
        {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
    poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
    poolInfo.pPoolSizes = pool_sizes;
    if (vkCreateDescriptorPool(Zayn->vkDevice, &poolInfo, nullptr,
                               &Zayn->imGuiDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create imgui descriptor pool!");
    }

    {
      // Attachment
      VkAttachmentDescription colorAttachment = {};
      colorAttachment.format = Zayn->vkSwapChainImageFormat;
      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // No MSAA
      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      // Color VkAttachmentReference our render pass needs.
      VkAttachmentReference colorAttachmentRef = {};
      colorAttachmentRef.attachment = 0;
      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      // subpass
      VkSubpassDescription subpass = {};
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &colorAttachmentRef;

      // synchronization and dependency
      VkSubpassDependency dependency = {};
      dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
      dependency.dstSubpass = 0;
      dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependency.srcAccessMask = 0; // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

      VkRenderPassCreateInfo info = {};
      info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      info.attachmentCount = 1;
      info.pAttachments = &colorAttachment;
      info.subpassCount = 1;
      info.pSubpasses = &subpass;
      info.dependencyCount = 1;
      info.pDependencies = &dependency;
      if (vkCreateRenderPass(Zayn->vkDevice, &info, nullptr, &Zayn->imGuiRenderPass) !=
          VK_SUCCESS) {
        throw std::runtime_error("Could not create Dear ImGui's render pass");
      }
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    
    // enable mouse cursor for ImGui
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.WantCaptureMouse = true;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForVulkan(Zayn->window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = Zayn->vkInstance;
    init_info.PhysicalDevice = Zayn->vkPhysicalDevice;
    init_info.Device = Zayn->vkDevice;
    init_info.QueueFamily = Zayn->vkQueueFamilyCount;
    init_info.Queue = Zayn->vkGraphicsQueue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = Zayn->imGuiDescriptorPool;
    init_info.Allocator = nullptr;
    init_info.RenderPass = Zayn->imGuiRenderPass;

    {
      // Already sized to the minImageCount + 1 &&  < MaxImageCount
      std::uint32_t imageCount = static_cast<uint32_t>(Zayn->vkSwapChainImages.size());
      init_info.MinImageCount = imageCount;
      init_info.ImageCount = imageCount;
    }
    // init_info.MinImageCount = g_MinImageCount;
    // init_info.ImageCount = wd->ImageCount;

    // init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);

    // VkCommandBuffer command_buffer = BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture();
    // EndSingleTimeCommands(command_buffer);

#endif


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(Zayn->window) && zaynPlatform.running)
    {

    //     /* Render here */
    //     /* Swap front and back buffers */

        //  *******************  //
        //  TIME IMPLEMENTATION  //
        //  *******************  //
        
    double systemTime = {};
        #ifdef __APPLE__
        clock_gettime(CLOCK_MONOTONIC, &spec);
        systemTime = (spec.tv_sec * 1000.0) + (spec.tv_nsec / 1.0e6);

    #else
        // Windows appropriate solution
        // Use QueryPerformanceCounter for high-resolution timer
        LARGE_INTEGER frequency;
        LARGE_INTEGER counter;
        if (!QueryPerformanceFrequency(&frequency))
        {
            // Handle error
        }
        if (!QueryPerformanceCounter(&counter))
        {
            // Handle error
        }
        systemTime = counter.QuadPart / static_cast<double>(frequency.QuadPart) * 1000.0;
    #endif   // Milliseconds


        deltaTime = (systemTime - prevSystemTime) / 1000.0;
        prevSystemTime = systemTime;

        gameTime = (systemTime - startTime) / 1000.0;

        // seconds
        double dt = deltaTime;

        timeSinceRender += dt;

        Zayn->deltaTime = dt;
        Zayn->time = gameTime;
        Zayn->systemTime = systemTime;


        // if (timeSinceRender < frameRate) 
        // {
        //     double timeUntilRender = frameRate - timeSinceRender;
        //     usleep(timeUntilRender * 1.0e6);
        // }

        InputUpdate(Zayn, inputManager);
        ZaynUpdateAndRender(zaynMemory);

       

        glfwSwapBuffers(Zayn->window);

        zaynPlatform.running = !glfwWindowShouldClose(Zayn->window);

        ClearInputManager(inputManager);

        /* Poll for and process events */

        timeSinceRender = 0.0;

    }

    RenderCleanup(Zayn);

    glfwDestroyWindow(Zayn->window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    // glViewport(0, 0, width, height);

    // window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);

    // glfwGetWindowUserPointer(window)
    // auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    auto zaynMem = reinterpret_cast<ZaynMemory*>(glfwGetWindowUserPointer(window));
    std::cout << "fbscb" << std::endl;
    zaynMem->vkFramebufferResized = true;
    // auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        // app->framebufferResized = true;
}