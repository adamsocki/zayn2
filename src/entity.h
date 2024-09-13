enum EntityType 
{
    EntityType_Entity, 
    EntityType_Monkey,
    EntityType_Floor,
    EntityType_Casette_Entity,

    EntityType_Count 
};


enum EntityFlag
{
    EntityFlag_None = 0,
    EntityFlag_Deleted = 1 << 2,
};

struct EntityTypeStorage 
{
    EntityType type;
    int32 elementSize;

    DynamicArray_Untyped array;
};

struct EntityInfo
{
    EntityType type;
    int32 generation;

    int32 index;
    EntityFlag flags;
};

struct EntityHandle
{
    EntityType type;
    uint32 id;
    int32 generation;
};

struct Transform2DComponent
{
    vec2 translation;
    mat2 iMatrix2 = Identity2();
    vec2 scale;
    real32 rotation;
};

struct Transform3DComponent
{
    vec3 translation;
    mat4 iMatrix4 = Identity4();
    vec3 scale;
    vec3 rotation;
    real32 angleRotation;
};


struct RenderComponent
{
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSet descriptorSet;
    VkDescriptorSetLayout descriptorSetLayout;
    uint32_t mipLevels;

    std::vector<VkPushConstantRange> pushConstantRanges;

    // std::vector<VkBuffer> uniformBuffers;
    
    std::vector<Vertex_> vertices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    std::vector<uint32_t> indices;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkBuffer instanceBuffer;
    VkDeviceMemory instanceBufferMemory;

    uint32_t indexCount;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;


    std::string modelPath;
    std::string texturePath;
    std::string shader_vert_path;
    std::string shader_frag_path;

    ShaderProgramPaths shaderPaths;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;


    bool hasIndexBuffer;
    bool hasModel;  
};

struct Entity
{
    EntityHandle handle;
    Model *model;
    vec3 color;
    Transform2DComponent transform2d;
    Transform3DComponent transform3d;
    RenderComponent renderComponent;
};

struct Player : public Entity {
    // char name[32];  // Player-specific data
    int32 level;
};

struct Casette_Entity : Entity
{

};

struct Monkey : Entity 
{
    bool testingBool;
    real32 rad;
    real32 posTest;
    

    // float patrolRange;  // Enemy-specific data
};


struct Floor : Entity
{
    // float friction;
    // float bounciness;
};

int32 EntitySizeTable[] = 
{
    sizeof(Entity),
    sizeof(Monkey),
    sizeof(Floor),
    sizeof(Casette_Entity),
    0,
};

int32 entityTypeCounts[] = 
{
    50, 
    20,
    1000,
    100
}; 



struct EntityManager
{
    DynamicArray<EntityInfo> entityInfo;
    DynamicArray<uint32> freelist;
    DynamicArray<EntityHandle> entitiesToDelete;

    EntityTypeStorage entities[EntityType_Count];
};


struct System 
{
    DynamicArray<EntityHandle> entities;
};

