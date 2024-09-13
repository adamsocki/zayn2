struct InstancedData
{
    mat4 modelMatrix;
};

struct Vertex_
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex_);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputBindingDescription, 2>  getBindingDescriptions_instanced()
    {
        std::array<VkVertexInputBindingDescription, 2> bindingDescriptions{};
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex_);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        bindingDescriptions[1].binding = 1;
        bindingDescriptions[1].stride = sizeof(InstancedData);
        bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

        return bindingDescriptions;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex_, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex_, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex_, texCoord);

        return attributeDescriptions;
    }

    static std::array<VkVertexInputAttributeDescription, 7> getAttributeDescriptions_instanced()
    {
        std::array<VkVertexInputAttributeDescription, 7> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex_, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex_, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex_, texCoord);

        attributeDescriptions[3].binding = 1; // Assuming binding 1 for instance data
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(InstancedData, modelMatrix) + sizeof(glm::vec4) * 0;

        attributeDescriptions[4].binding = 1; // Assuming binding 1 for instance data
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(InstancedData, modelMatrix) + sizeof(glm::vec4) * 1;

        attributeDescriptions[5].binding = 1; // Assuming binding 1 for instance data
        attributeDescriptions[5].location = 5;
        attributeDescriptions[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[5].offset = offsetof(InstancedData, modelMatrix) + sizeof(glm::vec4) * 2;

        attributeDescriptions[6].binding = 1; // Assuming binding 1 for instance data
        attributeDescriptions[6].location = 6;
        attributeDescriptions[6].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributeDescriptions[6].offset = offsetof(InstancedData, modelMatrix) + sizeof(glm::vec4) * 3;

        return attributeDescriptions;
    }

    bool operator==(const Vertex_& other) const {
    return pos == other.pos && color == other.color && texCoord == other.texCoord;
}
};
#include <unordered_map>

#include <memory>
#include <vector>


struct Model
{
    VkDevice *vkDevice;
    bool hasIndexBuffer = false;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    int32 vertexCount;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    int32 indexCount;
};


struct ShaderProgramPaths
{
    std::string vertPath;
    std::string fragPath;
};