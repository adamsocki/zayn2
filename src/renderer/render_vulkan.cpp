#include <vector>
#include <map>
#include <set>
#include <optional> // Add this line to include the <optional> header
#include <fstream> 

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#if _WIN32
const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#elif __APPLE__
const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#endif

const int MAX_FRAMES_IN_FLIGHT = 3;




std::string getShaderPath(const std::string& filename) {
#ifdef _WIN32
    return "C:/dev_c/zayn/src/renderer/shaders/" + filename;
#else
    return "/Users/socki/dev/zayn/src/renderer/shaders/" + filename;
#endif
}



const std::string MODEL_PATH =  getShaderPath( "models/viking_room.obj");
const std::string TEXTURE_PATH =  getShaderPath( "models/textures/viking_room.png");

std::string getTexturePath(const std::string& filename) {
#ifdef _WIN32
    return "C:/dev_c/zayn/models/textures/" + filename;
#else
    return "/Users/socki/dev/zayn/models/textures/" + filename;
#endif
}

std::string getModelPath(const std::string& filename) {
#ifdef _WIN32
    return "C:/dev_c/zayn/models/" + filename;
#else
    return "/Users/socki/dev/zayn/models/" + filename;
#endif
}

void PrintGPUName(VkInstance instance)
{
    // Enumerate physical devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Print the name of each physical device
    for (const auto &device : devices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        std::cout << "GPU: " << deviceProperties.deviceName << std::endl;
    }
}



VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

bool checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    VkLayerProperties *availableLayers = (VkLayerProperties *)malloc(layerCount * sizeof(VkLayerProperties));

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (const char *layerName : validationLayers)
    {
        bool layerFound = false;

        for (int j = 0; j < layerCount; j++)
        {
            if (strcmp(layerName, availableLayers[j].layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            free(availableLayers);
            return false;
        }
    }
    free(availableLayers);
    return true;
}

std::vector<const char *> getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #ifdef __APPLE__
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        #endif
    }
#ifdef __APPLE__
    uint32_t instanceExtensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);
    std::vector<VkExtensionProperties> availableInstanceExtensions(instanceExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, availableInstanceExtensions.data());
    for (const auto &extension : availableInstanceExtensions)
    {
        if (strcmp(extension.extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0)
        {
            extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            break;
        }
    }
#endif
    return extensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

void SetupDebugMessenger()
{
    if (!enableValidationLayers)
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    // populateDebugMessengerCreateInfo(createInfo);
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional


    VkAllocationCallbacks pAllocator = {};


#ifdef __APPLE__
    if (CreateDebugUtilsMessengerEXT(Zayn->vkInstance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), &pAllocator, &Zayn->vkDebugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
#else
    if (CreateDebugUtilsMessengerEXT(Zayn->vkInstance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(&createInfo), nullptr, &Zayn->vkDebugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
#endif

}

int rateDeviceSuitability(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;
    std::cout << "Device prop type : " << deviceProperties.deviceType << std::endl;
    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
        std::cout << "score should inc 1000 : " << std::endl;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;
    std::cout << "deviceProperties.limits.maxImageDimension2D : " << deviceProperties.limits.maxImageDimension2D << std::endl;

// Application can't function without geometry shaders
#ifdef __APPLE__ // Or another platform-specific check
    // Shader code without geometry shaders
#else
    if (!deviceFeatures.geometryShader)
    {
        std::cout << "!deviceFeatures.geometryShader : " << std::endl;
        return 0;
    }
// Shader code with geometry shaders
#endif
    return score;
}

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features, ZaynMemory *zaynMem)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(zaynMem->vkPhysicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat FindDepthFormat(ZaynMemory *zaynMem)
{
    return FindSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, zaynMem);
}

bool HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, ZaynMemory *zaynMem)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &Zayn->vkQueueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(Zayn->vkQueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &Zayn->vkQueueFamilyCount, queueFamilies.data());

    int i{};
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, zaynMem->vkSurface, &presentSupport);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, ZaynMemory *zaynMem)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, zaynMem->vkSurface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, zaynMem->vkSurface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, zaynMem->vkSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, zaynMem->vkSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, zaynMem->vkSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool isDeviceSuitable(VkPhysicalDevice device, ZaynMemory *zaynMem)
{
    QueueFamilyIndices indices = findQueueFamilies(device, zaynMem);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, zaynMem);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void printDeviceExtensions(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::cout << "Available extensions:\n";
    for (const auto &extension : availableExtensions)
    {
        std::cout << "\t" << extension.extensionName << "\n";
    }
}

void PickPhysicalDevice(ZaynMemory *zaynMem)
{

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(zaynMem->vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(zaynMem->vkInstance, &deviceCount, devices.data());

    for (const auto &device : devices)
    {
        // printDeviceExtensions(device);
        if (isDeviceSuitable(device, zaynMem))
        {
            zaynMem->vkPhysicalDevice = device;
            break;
        }
    }

    if (zaynMem->vkPhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void CreateVKInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        std::cerr << "Validation layers requested, but not available!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Zayn";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Zayn Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &Zayn->vkInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
    PrintGPUName(Zayn->vkInstance);
}

void CreateLogicalDevice(ZaynMemory *zaynMem)
{
    QueueFamilyIndices indices = findQueueFamilies(zaynMem->vkPhysicalDevice, zaynMem);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(zaynMem->vkPhysicalDevice, &createInfo, nullptr, &zaynMem->vkDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(zaynMem->vkDevice, indices.graphicsFamily.value(), 0, &zaynMem->vkGraphicsQueue);
    vkGetDeviceQueue(zaynMem->vkDevice, indices.presentFamily.value(), 0, &zaynMem->vkPresentQueue);
}

void CreateSurface()
{
    if (glfwCreateWindowSurface(Zayn->vkInstance, Zayn->window, nullptr, &Zayn->vkSurface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            std::cout << "Present mode: Mailbox" << std::endl;
            return availablePresentMode;
        }
        // UNCOMMENT BELOW IF YOU WANT TO NOT HAVE VSYNC ON
        else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            std::cout << "Present mode: Immediate (No VSync)" << std::endl;
            return availablePresentMode;
        }
    }
    std::cout << "Present mode: V-Sync" << std::endl;
    return VK_PRESENT_MODE_FIFO_KHR;
}
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, ZaynMemory *zaynMem)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(zaynMem->window, &width, &height);

        VkExtent2D actualExtent =
            {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void CreateSwapChain(ZaynMemory *zaynMem)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(zaynMem->vkPhysicalDevice, zaynMem);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, zaynMem);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = zaynMem->vkSurface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Use VK_IMAGE_USAGE_TRANSFER_DST_BIT for post-processing | use a memory operation to transfer the rendered image to a swap chain image.

    QueueFamilyIndices indices = findQueueFamilies(zaynMem->vkPhysicalDevice, zaynMem);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    VkSwapchainKHR oldSwapChain = zaynMem->vkSwapChain;

    if (oldSwapChain == VK_NULL_HANDLE)
    {
        createInfo.oldSwapchain = VK_NULL_HANDLE; // No existing swapchain
    }
    else
    {
        createInfo.oldSwapchain = oldSwapChain; // Use the existing swapchain
    }

    if (vkCreateSwapchainKHR(zaynMem->vkDevice, &createInfo, nullptr, &zaynMem->vkSwapChain) != VK_SUCCESS) // Make new swapchain
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, &imageCount, nullptr);
    zaynMem->vkSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, &imageCount, zaynMem->vkSwapChainImages.data());

    zaynMem->vkSwapChainImageFormat = surfaceFormat.format;
    zaynMem->vkSwapChainExtent = extent;
}

VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, ZaynMemory *zaynMem)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(zaynMem->vkDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void CreateImageViews(ZaynMemory *zaynMem)
{
    zaynMem->vkSwapChainImageViews.resize(zaynMem->vkSwapChainImages.size());

    for (size_t i = 0; i < zaynMem->vkSwapChainImages.size(); i++)
    {
        zaynMem->vkSwapChainImageViews[i] = CreateImageView(zaynMem->vkSwapChainImages[i], zaynMem->vkSwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, zaynMem);
  }
}

static std::vector<char> readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule createShaderModule(const std::vector<char> &code, ZaynMemory *zaynMem)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(Zayn->vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void CreateRenderPass(ZaynMemory *zaynMem)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = zaynMem->vkSwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    // changed for imgui
    #if IMGUI
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    #else
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    #endif



    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat(zaynMem);
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(zaynMem->vkDevice, &renderPassInfo, nullptr, &zaynMem->vkRenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }

    #if IMGUI

    
      // Attachment
      colorAttachment = {};
      colorAttachment.format = Zayn->vkSwapChainImageFormat;
      colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // No MSAA
      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
      colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

      // Color VkAttachmentReference our render pass needs.
      colorAttachmentRef = {};
      colorAttachmentRef.attachment = 0;
      colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      // subpass
      subpass = {};
      subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpass.colorAttachmentCount = 1;
      subpass.pColorAttachments = &colorAttachmentRef;

      // synchronization and dependency
      dependency = {};
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

    #endif
}

VkShaderModule LoadShader(const std::string &shaderFilePath)
{
    auto shaderCode = readFile(shaderFilePath);

    VkShaderModule shaderModule = createShaderModule(shaderCode, Zayn);

    return shaderModule;
}

void CreateGraphicsPipeline(ZaynMemory *zaynMem, VkPipeline *pipeline, const std::string &vertShaderFilePath, const std::string &fragShaderFilePath, std::vector<VkPushConstantRange> pushConstants, VkDescriptorSetLayout *descriptorSetLayout, VkPipelineLayout *pipelineLayout)
{
    auto vertShaderCode = readFile(vertShaderFilePath);
    auto fragShaderCode = readFile(fragShaderFilePath);

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, zaynMem);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, zaynMem);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex_::getBindingDescription();
    auto attributeDescriptions = Vertex_::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional

    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {};  // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayout;

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(ModelPushConstant);
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(zaynMem->vkDevice, &pipelineLayoutInfo, nullptr, pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = *pipelineLayout;
    pipelineInfo.renderPass = zaynMem->vkRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(zaynMem->vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(zaynMem->vkDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(zaynMem->vkDevice, vertShaderModule, nullptr);
}

void CreateGraphicsPipeline_Instances(ZaynMemory *zaynMem, VkPipeline *pipeline, const std::string &vertShaderFilePath, const std::string &fragShaderFilePath, std::vector<VkPushConstantRange> pushConstants, VkDescriptorSetLayout *descriptorSetLayout, VkPipelineLayout *pipelineLayout)
{
    auto vertShaderCode = readFile(vertShaderFilePath);
    auto fragShaderCode = readFile(fragShaderFilePath);

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, zaynMem);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, zaynMem);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescriptions = Vertex_::getBindingDescriptions_instanced();
    auto attributeDescriptions = Vertex_::getAttributeDescriptions_instanced();

    vertexInputInfo.vertexBindingDescriptionCount = 2;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional

    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {};  // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayout;

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(ModelPushConstant);
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(zaynMem->vkDevice, &pipelineLayoutInfo, nullptr, pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = *pipelineLayout;
    pipelineInfo.renderPass = zaynMem->vkRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(zaynMem->vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(zaynMem->vkDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(zaynMem->vkDevice, vertShaderModule, nullptr);
}

void CreateFrameBuffers()
{
    Zayn->vkSwapChainFramebuffers.resize(Zayn->vkSwapChainImageViews.size());

    #if IMGUI 

    Zayn->imGuiFrameBuffers.resize(Zayn->vkSwapChainImageViews.size());

    #endif

    for (size_t i = 0; i < Zayn->vkSwapChainImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {
            Zayn->vkSwapChainImageViews[i],
            Zayn->vkDepthImageView};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = Zayn->vkRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = Zayn->vkSwapChainExtent.width;
        framebufferInfo.height = Zayn->vkSwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(Zayn->vkDevice, &framebufferInfo, nullptr, &Zayn->vkSwapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }

#if IMGUI

        // Imgui framebuffer
        VkImageView imgui_attachment[1];
        framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = Zayn->imGuiRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = imgui_attachment;
        framebufferInfo.width = Zayn->vkSwapChainExtent.width;
        framebufferInfo.height = Zayn->vkSwapChainExtent.height;
        framebufferInfo.layers = 1;
        imgui_attachment[0] = Zayn->vkSwapChainImageViews[i];
        if (vkCreateFramebuffer(Zayn->vkDevice, &framebufferInfo, nullptr,
                                &Zayn->imGuiFrameBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create ImGui framebuffer!");
        }

#endif
    }
}

void CreateCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(Zayn->vkPhysicalDevice, Zayn);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags =  VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    // poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(Zayn->vkDevice, &poolInfo, nullptr, &Zayn->vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    #if IMGUI

    if (vkCreateCommandPool(Zayn->vkDevice, &poolInfo, nullptr, &Zayn->imGuiCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }

    #endif
}

void RecordCommandBuffer(ZaynMemory *zaynMem, VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
}

VkCommandBuffer BeginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Zayn->vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(Zayn->vkDevice, &allocInfo, &commandBuffer);

//     #ifdef IMGUI
//     Zayn->imGuiCommandBuffers.resize(
//         Zayn->vkSwapChainImageViews.size());

//         allocInfo = {};
//         allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//         allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//         allocInfo.commandPool = Zayn->imGuiCommandPool;
//         allocInfo.commandBufferCount =
//             static_cast<uint32_t>(Zayn->imGuiCommandBuffers.size());
//         vkAllocateCommandBuffers(Zayn->vkDevice, &allocInfo, Zayn->imGuiCommandBuffers.data());

// #endif

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Zayn->vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Zayn->vkGraphicsQueue);

    vkFreeCommandBuffers(Zayn->vkDevice, Zayn->vkCommandPool, 1, &commandBuffer);
}

void CreateCommandBuffers()
{
    // Zayn->vkCommandBuffers = (VkCommandBuffer *)malloc(sizeof(VkCommandBuffer) * MAX_FRAMES_IN_FLIGHT);
    Zayn->vkCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = Zayn->vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    if (vkAllocateCommandBuffers(Zayn->vkDevice, &allocInfo, Zayn->vkCommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
#if IMGUI
    // ImGui Command Buffer
    // Zayn->imGuiCommandBuffers.resize(    
    //     Zayn->vkSwapChainImageViews.size()); // or MAX_FRAMES_IN_FLIGHT?
    Zayn->imGuiCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    // create command buffers
    allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Zayn->imGuiCommandPool;
    allocInfo.commandBufferCount =
        static_cast<uint32_t>(Zayn->imGuiCommandBuffers.size());
    vkAllocateCommandBuffers(Zayn->vkDevice, &allocInfo, Zayn->imGuiCommandBuffers.data());
#endif
}

void CreateSyncObjects()
{
    Zayn->vkImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    Zayn->vkRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    Zayn->vkInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    Zayn->vkImagesInFlight.resize(Zayn->vkSwapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(Zayn->vkDevice, &semaphoreInfo, nullptr, &Zayn->vkImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(Zayn->vkDevice, &semaphoreInfo, nullptr, &Zayn->vkRenderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(Zayn->vkDevice, &fenceInfo, nullptr, &Zayn->vkInFlightFences[i]) != VK_SUCCESS)
        {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, ZaynMemory *zaynMem)
{
    // VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(zaynMem->vkPhysicalDevice, &Zayn->vkMemProperties);
    for (uint32_t i = 0; i < Zayn->vkMemProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (Zayn->vkMemProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage |VK_BUFFER_CREATE_SPARSE_BINDING_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Zayn->vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Zayn->vkDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties, Zayn);

    if (vkAllocateMemory(Zayn->vkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(Zayn->vkDevice, buffer, bufferMemory, 0);
}

void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, ZaynMemory *zaynMem)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);

    EndSingleTimeCommands(commandBuffer);
}

void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, ZaynMemory *zaynMem)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1};

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region);

    EndSingleTimeCommands(commandBuffer);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, ZaynMemory *zaynMem)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(commandBuffer);
}

void CreateImage(uint32_t width,
                 uint32_t height,
                 uint32_t mipLevels,
                 VkFormat format,
                 VkImageTiling tiling,
                 VkImageUsageFlags usage,
                 VkMemoryPropertyFlags properties,
                 VkImage &image,
                 VkDeviceMemory &imageMemory,
                 ZaynMemory *zaynMem)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(zaynMem->vkDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(zaynMem->vkDevice, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties, zaynMem);

    if (vkAllocateMemory(zaynMem->vkDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(zaynMem->vkDevice, image, imageMemory, 0);
}

void CreateDepthResources(ZaynMemory *zaynMem)
{
    VkFormat depthFormat = FindDepthFormat(zaynMem);

    CreateImage(zaynMem->vkSwapChainExtent.width, zaynMem->vkSwapChainExtent.height, 1, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, zaynMem->vkDepthImage, zaynMem->vkDepthImageMemory, zaynMem);
    zaynMem->vkDepthImageView = CreateImageView(zaynMem->vkDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1, zaynMem);
}

void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, ZaynMemory *zaynMem)
{
    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(zaynMem->vkPhysicalDevice, imageFormat, &formatProperties);

    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
    {
        throw std::runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < mipLevels; i++)
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = {0, 0, 0};
        blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = {0, 0, 0};
        blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(commandBuffer,
                       image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                       image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       1, &blit,
                       VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        if (mipWidth > 1)
            mipWidth /= 2;
        if (mipHeight > 1)
            mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer,
                         VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                         0, nullptr,
                         0, nullptr,
                         1, &barrier);

    EndSingleTimeCommands(commandBuffer);
}

void CreateTextureImage(uint32_t &mipLevels, VkImage *textureImage, VkDeviceMemory *textureImageMemory, const std::string texturePath)
{
    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load(texturePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
    mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

    if (!pixels)
    {
        throw std::runtime_error("failed to load texture image!");
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void *data;
    vkMapMemory(Zayn->vkDevice, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(Zayn->vkDevice, stagingBufferMemory);

    stbi_image_free(pixels);

    CreateImage(texWidth, texHeight, mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *textureImage, *textureImageMemory, Zayn);

    // TransitionImageLayout(zaynMem->vkTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, zaynMem);
    TransitionImageLayout(*textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels, Zayn);
    CopyBufferToImage(stagingBuffer, *textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), Zayn);

    vkDestroyBuffer(Zayn->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(Zayn->vkDevice, stagingBufferMemory, nullptr);
    GenerateMipmaps(*textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels, Zayn);
}

void CreateTextureImageView(uint32_t &mipLevels, VkImage *textureImage, VkImageView *textureImageView)
{
    *textureImageView = CreateImageView(*textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels, Zayn);
}

void CreateTextureSampler(uint32_t &mipLevels, VkSampler *textureSampler)
{
    VkPhysicalDeviceProperties properties = {};
    vkGetPhysicalDeviceProperties(Zayn->vkPhysicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(mipLevels);
    samplerInfo.mipLodBias = 0.0f;

    if (vkCreateSampler(Zayn->vkDevice, &samplerInfo, nullptr, textureSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void CreateVertexBuffer(std::vector<Vertex_> &vertices, VkBuffer *vertexBuffer, VkDeviceMemory *vertexBufferMemory)
{
    if (vertices.empty())
    {
        return;
    }
    VkDeviceSize bufferSize = sizeof(Vertex_) * vertices.size();
    // STAGING BUFFER - CPU accessible memory to upload the data from the vertex array to.
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(Zayn->vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(Zayn->vkDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *vertexBuffer, *vertexBufferMemory);

    copyBuffer(stagingBuffer, *vertexBuffer, bufferSize, Zayn);

    vkDestroyBuffer(Zayn->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(Zayn->vkDevice, stagingBufferMemory, nullptr);
}


void CreateInstanceBuffer(int32 instanceCount, VkBuffer *instanceBuffer, VkDeviceMemory *instanceBufferMemory)
{
    VkDeviceSize instancedDataSize = instanceCount * sizeof(InstancedData);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(instancedDataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    std::vector<InstancedData> instancedData(instanceCount);

    // Fill the instance data buffer with data
    for (uint32_t i = 0; i < instanceCount; i++)
    {
        real32 posx = i * 1.0f;
        instancedData[i].modelMatrix = TRS(V3(0.0f, 0.0f, posx), AxisAngle(V3(0.0f, 1.0f, 0.0f), DegToRad(0.0f)), V3(1.0f, 1.0f, 1.0f));
    }

    void* data;
    vkMapMemory(Zayn->vkDevice, stagingBufferMemory, 0, instancedDataSize, 0, &data);
    memcpy(data, instancedData.data(), instancedDataSize);
    vkUnmapMemory(Zayn->vkDevice, stagingBufferMemory);

    createBuffer(instancedDataSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *instanceBuffer, *instanceBufferMemory);

    copyBuffer(stagingBuffer, *instanceBuffer, instancedDataSize, Zayn);

    vkDestroyBuffer(Zayn->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(Zayn->vkDevice, stagingBufferMemory, nullptr);
}

void CreateIndexBuffer(std::vector<uint32_t> indices, VkBuffer *indexBuffer, VkDeviceMemory *indexBufferMemory)
{
    if (indices.empty())
    {
        return;
    }
    VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void *data;
    vkMapMemory(Zayn->vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(Zayn->vkDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, *indexBuffer, *indexBufferMemory);

    copyBuffer(stagingBuffer, *indexBuffer, bufferSize, Zayn);

    vkDestroyBuffer(Zayn->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(Zayn->vkDevice, stagingBufferMemory, nullptr);
}

void CreateDescriptorSetLayout(VkDescriptorSetLayout *descriptorSetLayout, bool hasImage)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings = {};
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    bindings.push_back(uboLayoutBinding);

    if (hasImage)
    {
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        bindings.push_back(samplerLayoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(Zayn->vkDevice, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void CreateUniformBuffer(std::vector<VkBuffer> &uniformBuffers, std::vector<VkDeviceMemory> &uniformBuffersMemory, std::vector<void *> &uniformBuffersMapped)
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        vkMapMemory(Zayn->vkDevice, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void CreateDescriptorPool(VkDescriptorPool *descriptorPool, bool hasImage)
{
    std::vector<VkDescriptorPoolSize> poolSizes = {};
    VkDescriptorPoolSize poolSize_1{};
    poolSize_1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize_1.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 20);
    poolSizes.push_back(poolSize_1);
    if (hasImage)
    {
        VkDescriptorPoolSize poolSize_2 = {};
        poolSize_2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize_2.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 20);
        poolSizes.push_back(poolSize_2);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * 20);

    if (vkCreateDescriptorPool(Zayn->vkDevice, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

template <typename T>
void CreatePushConstant()
{
    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(T);

    Zayn->vkPushConstantRanges.push_back(pushConstantRange);
}

void CreateDescriptorSets(bool hasImage, size_t uniformBufferSize, std::vector<VkBuffer> &uniformBuffers, VkDescriptorSetLayout *descriptorSetLayout, VkDescriptorPool *descriptorPool, std::vector<VkDescriptorSet> &descriptorSets, VkImageView *textureImageView, VkSampler *textureSampler)
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = *descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(Zayn->vkDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = uniformBufferSize;

        std::vector<VkWriteDescriptorSet> descriptorWrites{};

        VkWriteDescriptorSet descriptorWrite_uniform{};
        descriptorWrite_uniform.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite_uniform.dstSet = descriptorSets[i];
        descriptorWrite_uniform.dstBinding = 0;
        descriptorWrite_uniform.dstArrayElement = 0;
        descriptorWrite_uniform.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite_uniform.descriptorCount = 1;
        descriptorWrite_uniform.pBufferInfo = &bufferInfo;
        descriptorWrites.push_back(descriptorWrite_uniform);

        if (hasImage)
        {
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = *textureImageView;
            imageInfo.sampler = *textureSampler;

            VkWriteDescriptorSet descriptorWrite_image{};
            descriptorWrite_image.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite_image.dstSet = descriptorSets[i];
            descriptorWrite_image.dstBinding = 1;
            descriptorWrite_image.dstArrayElement = 0;
            descriptorWrite_image.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrite_image.descriptorCount = 1;
            descriptorWrite_image.pImageInfo = &imageInfo;
            descriptorWrites.push_back(descriptorWrite_image);
        }

        vkUpdateDescriptorSets(Zayn->vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}



void StartRender_Init()
{
    CreateVKInstance();
    SetupDebugMessenger();
    CreateSurface(); // is SAME
    PickPhysicalDevice(Zayn);  // is SAME  // <-- @TODO: May need to modify to account for other machines like rPi
    CreateLogicalDevice(Zayn); // is SAME
    CreateSwapChain(Zayn);
    CreateImageViews(Zayn);
    CreateRenderPass(Zayn);

    CreateCommandPool();
    CreateDepthResources(Zayn);
    CreateFrameBuffers();
}

void EndRender_Init()
{

    CreateCommandBuffers();
    CreateSyncObjects();
}

void InitRender_Learn()
{
    StartRender_Init();

    CreateDescriptorSetLayout(&Zayn->vkDescriptorSetLayout, true);
    CreateDescriptorSetLayout(&Zayn->vkDescriptorSetLayout_blank, false);

    // CreatePushConstant();
    CreatePushConstant<ModelPushConstant>();
   
    // VkPushConstantRange pushConstantRange = {};
    // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    // pushConstantRange.offset = 0;
    // pushConstantRange.size = sizeof(ModelPushConstant);

    // Zayn->vkPushConstantRanges.push_back(pushConstantRange);

    // LIST OF SHADERS

    
    CreateGraphicsPipeline(Zayn, &Zayn->vkGraphicsPipeline,  getShaderPath("vkShader_3d_INIT_vert.spv"),  getShaderPath( "vkShader_3d_INIT_frag.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout, &Zayn->vkPipelineLayout);
    // CreateGraphicsPipeline(Zayn, &Zayn->monkey_RC.pipeline,  getShaderPath( "vkShader_3d_INIT_vert.spv"),  getShaderPath( "vkShader_3d_INIT_frag.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout, &Zayn->vkPipelineLayout);
    CreateGraphicsPipeline_Instances(Zayn, &Zayn->monkey_RC.pipeline,  getShaderPath( "vkShader_3d_INIT_vert_inst.spv"),  getShaderPath( "vkShader_3d_INIT_frag.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout, &Zayn->vkPipelineLayout);
    CreateGraphicsPipeline(Zayn, &Zayn->sphereRC.pipeline,  getShaderPath( "vkShader_3d_INIT_vert.spv"),  getShaderPath( "vkShader_3d_INIT_frag.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout, &Zayn->vkPipelineLayout);
    CreateGraphicsPipeline(Zayn, &Zayn->sphereRC_blank.pipeline,  getShaderPath( "vert_blank.spv"),  getShaderPath( "frag_blank.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout_blank, &Zayn->vkPipelineLayout_blank);
    CreateGraphicsPipeline(Zayn, &Zayn->grid.pipeline,  getShaderPath( "grid_vert.spv"),  getShaderPath( "grid_frag.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout_blank, &Zayn->vkPipelineLayout_blank);

    CreateTextureImage(Zayn->vkMipLevels, &Zayn->vkTextureImage, &Zayn->vkTextureImageMemory,  getTexturePath( "viking_room.png"));
    CreateTextureImage(Zayn->monkey_RC.mipLevels, &Zayn->monkey_RC.textureImage, &Zayn->monkey_RC.textureImageMemory,  getTexturePath( "viking_room.png"));
    CreateTextureImage(Zayn->sphereRC.mipLevels, &Zayn->sphereRC.textureImage, &Zayn->sphereRC.textureImageMemory,  getTexturePath( "viking_room.png"));

    CreateTextureImageView(Zayn->vkMipLevels, &Zayn->vkTextureImage, &Zayn->vkTextureImageView);
    CreateTextureImageView(Zayn->monkey_RC.mipLevels, &Zayn->monkey_RC.textureImage, &Zayn->monkey_RC.textureImageView);
    CreateTextureImageView(Zayn->sphereRC.mipLevels, &Zayn->sphereRC.textureImage, &Zayn->sphereRC.textureImageView);

    CreateTextureSampler(Zayn->vkMipLevels, &Zayn->vkTextureSampler);
    CreateTextureSampler(Zayn->monkey_RC.mipLevels, &Zayn->monkey_RC.textureSampler);
    CreateTextureSampler(Zayn->sphereRC.mipLevels, &Zayn->sphereRC.textureSampler);

    LoadModel( getModelPath( "gwc.obj"), &Zayn->vkVertices, &Zayn->vkIndices);
    LoadModel( getModelPath( "viking_room.obj"), &Zayn->monkey_RC.vertices, &Zayn->monkey_RC.indices);
    LoadModel( getModelPath( "sphere.obj"), &Zayn->sphereRC.vertices, &Zayn->sphereRC.indices);
    LoadModel( getModelPath( "sphere.obj"), &Zayn->sphereRC_blank.vertices, &Zayn->sphereRC_blank.indices);
    LoadModel( getModelPath( "grid_plane.obj"), &Zayn->grid.vertices, &Zayn->grid.indices);

    CreateVertexBuffer(Zayn->vkVertices, &Zayn->vkVertexBuffer, &Zayn->vkVertexBufferMemory);
    CreateVertexBuffer(Zayn->monkey_RC.vertices, &Zayn->monkey_RC.vertexBuffer, &Zayn->monkey_RC.vertexBufferMemory);
    CreateVertexBuffer(Zayn->sphereRC.vertices, &Zayn->sphereRC.vertexBuffer, &Zayn->sphereRC.vertexBufferMemory);
    CreateVertexBuffer(Zayn->sphereRC_blank.vertices, &Zayn->sphereRC_blank.vertexBuffer, &Zayn->sphereRC_blank.vertexBufferMemory);
    CreateVertexBuffer(Zayn->grid.vertices, &Zayn->grid.vertexBuffer, &Zayn->grid.vertexBufferMemory);

    CreateInstanceBuffer(20, &Zayn->monkey_RC.instanceBuffer, &Zayn->monkey_RC.instanceBufferMemory);

    CreateIndexBuffer(Zayn->vkIndices, &Zayn->vkIndexBuffer, &Zayn->vkIndexBufferMemory);
    CreateIndexBuffer(Zayn->monkey_RC.indices, &Zayn->monkey_RC.indexBuffer, &Zayn->monkey_RC.indexBufferMemory);
    CreateIndexBuffer(Zayn->sphereRC.indices, &Zayn->sphereRC.indexBuffer, &Zayn->sphereRC.indexBufferMemory);
    CreateIndexBuffer(Zayn->sphereRC_blank.indices, &Zayn->sphereRC_blank.indexBuffer, &Zayn->sphereRC_blank.indexBufferMemory);
    CreateIndexBuffer(Zayn->grid.indices, &Zayn->grid.indexBuffer, &Zayn->grid.indexBufferMemory);

    CreateUniformBuffer(Zayn->vkUniformBuffers, Zayn->vkUniformBuffersMemory, Zayn->vkUniformBuffersMapped);

    CreateDescriptorPool(&Zayn->vkDescriptorPool, true);      // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    CreateDescriptorPool(&Zayn->descriptorPool_blank, false); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED

    CreateDescriptorSets(true, sizeof(UniformBufferObject), Zayn->vkUniformBuffers, &Zayn->vkDescriptorSetLayout, &Zayn->vkDescriptorPool, Zayn->vkDescriptorSets, &Zayn->vkTextureImageView, &Zayn->vkTextureSampler);
    CreateDescriptorSets(true, sizeof(UniformBufferObject), Zayn->vkUniformBuffers, &Zayn->vkDescriptorSetLayout, &Zayn->vkDescriptorPool, Zayn->vkDescriptorSets, &Zayn->monkey_RC.textureImageView, &Zayn->monkey_RC.textureSampler);
    CreateDescriptorSets(false, sizeof(UniformBufferObject), Zayn->vkUniformBuffers, &Zayn->vkDescriptorSetLayout_blank, &Zayn->descriptorPool_blank, Zayn->vkDescriptorSets_blank, nullptr, nullptr);
    // CreateDescriptorSets(false, sizeof(UniformBufferObject_x1), Zayn->vkUniformBuffers, &Zayn->vkDescriptorSetLayout_blank, &Zayn->descriptorPool_blank, Zayn->vkDescriptorSets_blank, nullptr, nullptr);

    EndRender_Init();
}

void CleanUpSwapChain(ZaynMemory *zaynMem)
{
    vkDestroyImageView(zaynMem->vkDevice, zaynMem->vkDepthImageView, nullptr);
    vkDestroyImage(zaynMem->vkDevice, zaynMem->vkDepthImage, nullptr);
    vkFreeMemory(zaynMem->vkDevice, zaynMem->vkDepthImageMemory, nullptr);

    for (size_t i = 0; i < zaynMem->vkSwapChainFramebuffers.size(); i++)
    {
        vkDestroyFramebuffer(zaynMem->vkDevice, zaynMem->vkSwapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < zaynMem->vkSwapChainImageViews.size(); i++)
    {
        vkDestroyImageView(zaynMem->vkDevice, zaynMem->vkSwapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, nullptr);
}
void RecreateSwapChain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(Zayn->window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(Zayn->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(Zayn->vkDevice);

    CleanUpSwapChain(Zayn);
    CreateSwapChain(Zayn);
    CreateImageViews(Zayn);
    CreateDepthResources(Zayn);
    CreateFrameBuffers();
}
// add a push constant to the pipeline

void UpdateUniformBuffer(uint32_t currentImage, ZaynMemory *zaynMem)
{
    // static auto startTime = std::chrono::high_resolution_clock::now();

    // auto currentTime = std::chrono::high_resolution_clock::now();
    // float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    Camera *cam = &Zayn->camera;

    UniformBufferObject ubo = {};
    // ubo.model = TRS(V3(0.0f, 0.0f, 0.0f), AxisAngle(V3(0.0f, 0.2f, 0.20f), time * DegToRad(10.0f)), V3(1.0f, 1.0f, 1.0f));
  
    // apply view based on camera rotation
    
    cam->front.x = cosf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));
    cam->front.y = sinf(DegToRad(cam->pitch));
    cam->front.z = sinf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));
    cam->front = normalize(cam->front);

    ubo.view = glm::lookAt(cam->pos, cam->pos + cam->front, cam->up);

    ubo.proj = glm::perspective(glm::radians(60.0f), zaynMem->vkSwapChainExtent.width / (float)zaynMem->vkSwapChainExtent.height, 0.1f, 1000.0f);
    ubo.proj[1][1] *= -1;
    

    memcpy(zaynMem->vkUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

VkResult AcquireNextImage(uint32_t *imageIndex)
{
    vkWaitForFences(Zayn->vkDevice, 1, &Zayn->vkInFlightFences[Zayn->vkCurrentFrame], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(
        Zayn->vkDevice,
        Zayn->vkSwapChain,
        UINT64_MAX,
        Zayn->vkImageAvailableSemaphores[Zayn->vkCurrentFrame], // must be a not signaled semaphore
        VK_NULL_HANDLE,
        imageIndex);

    return result;
}

bool BeginFrame()
{
    assert(!Zayn->vkIsFrameStarted && "cannot call begin frame when frame buffer is already in progress");
    auto result = AcquireNextImage(&Zayn->vkCurrentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapChain();
        return false;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    Zayn->vkIsFrameStarted = true;

    // vkResetFences(Zayn->vkDevice, 1, &Zayn->vkInFlightFences[Zayn->vkCurrentFrame]);
    // vkResetCommandBuffer(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], /*VkCommandBufferResetFlagBits*/ 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
        return false;
    }

    return true;
}

void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(Zayn->vkIsFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == Zayn->vkCommandBuffers[Zayn->vkCurrentFrame] && "Can't begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = Zayn->vkRenderPass;
    renderPassInfo.framebuffer = Zayn->vkSwapChainFramebuffers[Zayn->vkCurrentImageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = Zayn->vkSwapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(Zayn->vkSwapChainExtent.width);
    viewport.height = static_cast<float>(Zayn->vkSwapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, Zayn->vkSwapChainExtent};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void RenderEntity_notYetEntity(VkCommandBuffer imageBuffer, VkPipeline *pipeline, VkPipelineLayout *pipelineLayout, std::vector<VkDescriptorSet> &descriptorSets, VkBuffer *vertexBuffer, VkBuffer *indexBuffer, std::vector<uint32_t> &indices, ModelPushConstant *pushConstant)
{
    vkCmdBindPipeline(imageBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
    vkCmdBindDescriptorSets(imageBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipelineLayout, 0, 1, &descriptorSets[Zayn->vkCurrentFrame], 0, nullptr);

    vkCmdPushConstants(imageBuffer, *pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ModelPushConstant), pushConstant);

    // Replace Below with BindModel() and DrawModel()

    VkBuffer vertexBuffers[] = {*vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(imageBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(imageBuffer, *indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(imageBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void RenderEntity_inst(VkCommandBuffer imageBuffer, VkPipeline *pipeline, VkPipelineLayout *pipelineLayout, std::vector<VkDescriptorSet> &descriptorSets, VkBuffer* instanceBuffer, VkBuffer *vertexBuffer, VkBuffer *indexBuffer, std::vector<uint32_t> &indices, ModelPushConstant *pushConstant)
{
    vkCmdBindPipeline(imageBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
    vkCmdBindDescriptorSets(imageBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipelineLayout, 0, 1, &descriptorSets[Zayn->vkCurrentFrame], 0, nullptr);

    vkCmdPushConstants(imageBuffer, *pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ModelPushConstant), pushConstant);

    // Replace Below with BindModel() and DrawModel()

    VkBuffer vertexBuffers[] = {*vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(imageBuffer, 0, 1, vertexBuffers, offsets);
    VkBuffer instanceBuffers[] = {*instanceBuffer};
    vkCmdBindVertexBuffers(imageBuffer, 1, 1, instanceBuffers, offsets);

    vkCmdBindIndexBuffer(imageBuffer, *indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdDrawIndexed(imageBuffer, static_cast<uint32_t>(indices.size()), 20, 0, 0, 0);
}
void EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    assert(Zayn->vkIsFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == Zayn->vkCommandBuffers[Zayn->vkCurrentFrame] && "Can't begin render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);
}

VkResult SubmitCommandBuffers(std::vector<VkCommandBuffer> buffers, uint32_t *imageIndex, ZaynMemory *zaynMem)
{
    
    if (zaynMem->vkImagesInFlight[*imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(zaynMem->vkDevice, 1, &zaynMem->vkImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
    }
    zaynMem->vkImagesInFlight[*imageIndex] = zaynMem->vkImagesInFlight[zaynMem->vkCurrentFrame];
    // if (zaynMem->vkImagesInFlight[Zayn->vkCurrentFrame] != VK_NULL_HANDLE)
    // {
    //     VkResult fenceStatus = vkGetFenceStatus(Zayn->vkDevice, Zayn->vkInFlightFences[Zayn->vkCurrentFrame]);
    //     if (fenceStatus == VK_SUCCESS)
    //     {
    //         // Fence is ready, reset it
    //         vkResetFences(Zayn->vkDevice, 1, &Zayn->vkInFlightFences[zaynMem->vkCurrentFrame]);
    //     }
    //     // Optionally handle the case where the fence is not ready
    // }

    // vkWaitForFences(zaynMem->vkDevice, 1, &zaynMem->vkImagesInFlight[Zayn->vkCurrentFrame], VK_TRUE, UINT64_MAX);
//     if (zaynMem->vkImagesInFlight[Zayn->vkCurrentFrame] != VK_NULL_HANDLE)
//     {
//         vkWaitForFences(zaynMem->vkDevice, 1, &zaynMem->vkImagesInFlight[Zayn->vkCurrentFrame], VK_TRUE, UINT64_MAX);
// // 
//     }
    // zaynMem->vkImagesInFlight[*imageIndex] = zaynMem->vkImagesInFlight[zaynMem->vkCurrentFrame];
    // vkResetFences(Zayn->vkDevice, 1, &Zayn->vkInFlightFences[zaynMem->vkCurrentFrame]);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {Zayn->vkImageAvailableSemaphores[Zayn->vkCurrentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = static_cast<uint32_t>(buffers.size());
    // submitInfo.commandBufferCount
    submitInfo.pCommandBuffers = buffers.data();

    VkSemaphore signalSemaphores[] = {Zayn->vkRenderFinishedSemaphores[Zayn->vkCurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // vkResetFences(device.device(), 1, &inFlightFences[currentFrame]);
    vkResetFences(Zayn->vkDevice, 1, &Zayn->vkInFlightFences[zaynMem->vkCurrentFrame]);
//     if (vkQueueSubmit(device.graphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) !=
//       VK_SUCCESS) {
//     throw std::runtime_error("failed to submit draw command buffer!");
//   }

    // VkResult fenceStatus = vkGetFenceStatus(Zayn->vkDevice, Zayn->vkInFlightFences[Zayn->vkCurrentFrame]);
    // if (fenceStatus == VK_NOT_READY)
    // {
    //     // Optionally: Skip frame or do something else while waiting
    // }
    // else
    // {
    //     vkResetFences(Zayn->vkDevice, 1, &Zayn->vkInFlightFences[zaynMem->vkCurrentFrame]);
    // }
    if (vkQueueSubmit(Zayn->vkGraphicsQueue, 1, &submitInfo, Zayn->vkInFlightFences[Zayn->vkCurrentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {Zayn->vkSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = imageIndex;
    // presentInfo.pImageIndices = &Zayn->vkCurrentImageIndex;

    auto result = vkQueuePresentKHR(Zayn->vkPresentQueue, &presentInfo);

    zaynMem->vkCurrentFrame = (zaynMem->vkCurrentFrame + 1) % zaynMem->VK_MAX_FRAMES_IN_FLIGHT;

    return result;
    
}
void EndFrame()
{
    assert(Zayn->vkIsFrameStarted && "Can't call endFrame while frame is not in progress");

    std::vector<VkCommandBuffer> submitCommandBuffers = {};

    submitCommandBuffers.push_back(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame]);

    #if IMGUI    
    submitCommandBuffers.push_back(Zayn->imGuiCommandBuffers[Zayn->vkCurrentFrame]);
    #endif

    if (vkEndCommandBuffer(submitCommandBuffers[0]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }

    auto result = SubmitCommandBuffers(submitCommandBuffers, &Zayn->vkCurrentImageIndex, Zayn);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Zayn->vkFramebufferResized)
    {
        Zayn->vkFramebufferResized = false;
        RecreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }
    Zayn->vkIsFrameStarted = false;
}

// Error Function used for ImGui code.
  static void check_vk_result(VkResult err) {
    if (err == 0)
      return;
    fprintf(stderr, "ImGui [vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
      abort();
  }

void DoIMGUI()
{
    // Recording ImGui Command Buffer
    {
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        VkResult err =
            vkBeginCommandBuffer(Zayn->imGuiCommandBuffers[Zayn->vkCurrentFrame], &info);
        check_vk_result(err);

        // Render pass
        {
            VkClearValue clearValue = {};
            clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
            // Copy passed clear color
            memcpy(&clearValue.color.float32[0], &clear_color, 4 * sizeof(float));

            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = Zayn->imGuiRenderPass;
            info.framebuffer = Zayn->imGuiFrameBuffers[Zayn->vkCurrentImageIndex];
            // swapChainFramebuffers[currentFrame];
            info.renderArea.extent.width = Zayn->vkSwapChainExtent.width;
            info.renderArea.extent.height = Zayn->vkSwapChainExtent.height;
            info.clearValueCount = 1;
            info.pClearValues = &clearValue;
            vkCmdBeginRenderPass(Zayn->imGuiCommandBuffers[Zayn->vkCurrentFrame], &info,
                                 VK_SUBPASS_CONTENTS_INLINE);
        }

        // ImGui Render command
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                        Zayn->imGuiCommandBuffers[Zayn->vkCurrentFrame]);
        // Submit command buffer
        vkCmdEndRenderPass(Zayn->imGuiCommandBuffers[Zayn->vkCurrentFrame]);
        {
            err = vkEndCommandBuffer(Zayn->imGuiCommandBuffers[Zayn->vkCurrentFrame]);
            check_vk_result(err);
        }
    }
}

void CreateVulkanRenderingForEntity(RenderComponent *rc, bool createDescriptorSet)
{
    CreateGraphicsPipeline(Zayn, &rc->pipeline, rc->shader_vert_path, rc->shader_frag_path, Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout_blank, &Zayn->vkPipelineLayout_blank);
    LoadModel(rc->modelPath, &rc->vertices, &rc->indices);
    CreateVertexBuffer(rc->vertices, &rc->vertexBuffer, &rc->vertexBufferMemory);
    CreateIndexBuffer(rc->indices, &rc->indexBuffer, &rc->indexBufferMemory);
    if (createDescriptorSet)
    {
        CreateDescriptorSets(true, sizeof(UniformBufferObject), Zayn->vkUniformBuffers, &Zayn->vkDescriptorSetLayout, &Zayn->vkDescriptorPool, Zayn->vkDescriptorSets, &Zayn->monkey_RC.textureImageView, &Zayn->monkey_RC.textureSampler);
    }
}

vec3 posModel1 = V3(0.0f, 0.0f, 0.0f);
vec3 posModel2 = V3(0.0f, 0.0f, 0.0f);
vec3 posModel3 = V3(0.0f, 0.0f, 0.0f);
vec3 posModel4 = V3(0.0f, 0.0f, 1.0f);
vec3 posModel5 = V3(0.0f, 1.0f, 1.0f);


void UpdateRender_Learn()
{

    if (BeginFrame())
    {
        // UPDATE
        UpdateUniformBuffer(Zayn->vkCurrentFrame, Zayn);
       
        BeginSwapChainRenderPass(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame]);
        ModelPushConstant pushConstantData1 = {};
        ModelPushConstant pushConstantData2 = {};
        ModelPushConstant pushConstantData3 = {};
        ModelPushConstant pushConstantData4 = {};
        ModelPushConstant pushConstantData5 = {};

        if (InputHeld(Keyboard, Input_K))
        {
            posModel1.x += 0.51f;
            posModel2.x -= 0.51f;
            posModel3.y -= 0.51f;
            posModel4.z -= 0.51f;
            posModel5.z += 0.51f;
        }
        if (InputHeld(Keyboard, Input_L))
        {
            posModel1.x -= 0.51f;
            posModel2.x += 0.51f;
            posModel3.y += 0.51f;
            posModel4.z += 0.51f;
            posModel5.z -= 0.51f;
        }

        pushConstantData1.model_1 = TRS((posModel1), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));
        pushConstantData2.model_1 = TRS((posModel2), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));
        pushConstantData3.model_1 = TRS((posModel3), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));
        pushConstantData4.model_1 = TRS((posModel4), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));
        pushConstantData5.model_1 = TRS((posModel5), AxisAngle(V3(0.0f, 0.0f, 0.0f), DegToRad(90.0f)), V3(11.0f, 11.0f, 11.0f));
        RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->vkGraphicsPipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets, &Zayn->vkVertexBuffer, &Zayn->vkIndexBuffer, Zayn->vkIndices, &pushConstantData1);
        // RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->monkey_RC.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets, &Zayn->monkey_RC.vertexBuffer, &Zayn->monkey_RC.indexBuffer, Zayn->monkey_RC.indices, &pushConstantData2);
        RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->sphereRC.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets, &Zayn->sphereRC.vertexBuffer, &Zayn->sphereRC.indexBuffer, Zayn->sphereRC.indices, &pushConstantData3);
        RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->sphereRC_blank.pipeline, &Zayn->vkPipelineLayout_blank, Zayn->vkDescriptorSets_blank, &Zayn->sphereRC_blank.vertexBuffer, &Zayn->sphereRC_blank.indexBuffer, Zayn->sphereRC_blank.indices, &pushConstantData4);
        RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->grid.pipeline, &Zayn->vkPipelineLayout_blank, Zayn->vkDescriptorSets_blank, &Zayn->grid.vertexBuffer, &Zayn->grid.indexBuffer, Zayn->grid.indices, &pushConstantData5);
        
        
        for (int i = 0; i < Zayn->renderComponents.count; i++) // RENDER FOR BLENDER MODELS IMPORTED
        {
            RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->renderComponents[i].pipeline, &Zayn->vkPipelineLayout_blank, Zayn->vkDescriptorSets_blank, &Zayn->renderComponents[i].vertexBuffer, &Zayn->renderComponents[i].indexBuffer, Zayn->renderComponents[i].indices, &pushConstantData1);
        }
        
        RenderEntity_inst(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->monkey_RC.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets, &Zayn->monkey_RC.instanceBuffer, &Zayn->monkey_RC.vertexBuffer, &Zayn->monkey_RC.indexBuffer, Zayn->monkey_RC.indices, &pushConstantData2);

        #if IMGUI

        DoIMGUI();

        #endif

        EndSwapChainRenderPass(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame]);

        EndFrame();
    }
}

void RenderCleanup(ZaynMemory *zaynMem)
{
    CleanUpSwapChain(zaynMem);

    vkDestroyImage(zaynMem->vkDevice, zaynMem->vkTextureImage, nullptr);
    vkFreeMemory(zaynMem->vkDevice, zaynMem->vkTextureImageMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(zaynMem->vkDevice, zaynMem->vkUniformBuffers[i], nullptr);
        vkFreeMemory(zaynMem->vkDevice, zaynMem->vkUniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(zaynMem->vkDevice, zaynMem->vkDescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(zaynMem->vkDevice, zaynMem->vkDescriptorSetLayout, nullptr);

    vkDestroyBuffer(zaynMem->vkDevice, zaynMem->vkVertexBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, zaynMem->vkVertexBufferMemory, nullptr);

    vkDestroyBuffer(zaynMem->vkDevice, zaynMem->vkIndexBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, zaynMem->vkIndexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(zaynMem->vkDevice, zaynMem->vkRenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(zaynMem->vkDevice, zaynMem->vkImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(zaynMem->vkDevice, zaynMem->vkInFlightFences[i], nullptr);
    }
    vkDestroyCommandPool(zaynMem->vkDevice, zaynMem->vkCommandPool, nullptr);
    for (auto framebuffer : zaynMem->vkSwapChainFramebuffers)
    {
        vkDestroyFramebuffer(zaynMem->vkDevice, framebuffer, nullptr);
    }
    vkDestroyPipeline(zaynMem->vkDevice, zaynMem->vkGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(zaynMem->vkDevice, zaynMem->vkPipelineLayout, nullptr);
    vkDestroyRenderPass(zaynMem->vkDevice, zaynMem->vkRenderPass, nullptr);
    for (auto imageView : zaynMem->vkSwapChainImageViews)
    {
        vkDestroyImageView(zaynMem->vkDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, nullptr);
    vkDestroyDevice(zaynMem->vkDevice, nullptr);
    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(zaynMem->vkInstance, zaynMem->vkDebugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(zaynMem->vkInstance, zaynMem->vkSurface, nullptr);
    vkDestroyInstance(zaynMem->vkInstance, nullptr);

    vkDestroyImageView(zaynMem->vkDevice, zaynMem->vkTextureImageView, nullptr);

    vkDestroyImage(zaynMem->vkDevice, zaynMem->vkTextureImage, nullptr);
    vkFreeMemory(zaynMem->vkDevice, zaynMem->vkTextureImageMemory, nullptr);

    vkDestroySampler(zaynMem->vkDevice, zaynMem->vkTextureSampler, nullptr);
    vkDestroyImageView(zaynMem->vkDevice, zaynMem->vkTextureImageView, nullptr);

    vkDeviceWaitIdle(zaynMem->vkDevice);

    // free(zaynMem->vkCommandBuffers);
}

