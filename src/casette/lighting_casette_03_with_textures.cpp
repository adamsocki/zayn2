


void CreateDescriptorSetLayout_03_Textures(VkDescriptorSetLayout *descriptorSetLayout)
{
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding uboMaterialLayoutBinding = {};
    uboMaterialLayoutBinding.binding = 1;
    uboMaterialLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    uboMaterialLayoutBinding.descriptorCount = 1;
    uboMaterialLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboMaterialLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding textureSamplerLayoutBinding{};
    textureSamplerLayoutBinding.binding = 2;
    textureSamplerLayoutBinding.descriptorCount = 1;
    textureSamplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    textureSamplerLayoutBinding.pImmutableSamplers = nullptr;
    textureSamplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 3> bindings = {uboLayoutBinding, uboMaterialLayoutBinding, textureSamplerLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(Zayn->vkDevice, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void CreateDescriptorSetLayout_forLight(VkDescriptorSetLayout *descriptorSetLayout)
{
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 1> bindings = {uboLayoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(Zayn->vkDevice, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void AllocateDescriptorSets(VkDescriptorSetLayout *descriptorSetLayout, VkDescriptorPool *descriptorPool, std::vector<VkDescriptorSet> &descriptorSets)
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
}

void CreateDescriptorSets_forTexture(size_t uniformBufferSize1, size_t uniformBufferSize2, std::vector<VkBuffer> &uniformBuffers1, std::vector<VkBuffer> &uniformBuffers2, VkDescriptorSetLayout *descriptorSetLayout, VkDescriptorPool *descriptorPool, std::vector<VkDescriptorSet> &descriptorSets)
{
    
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo1{};
        bufferInfo1.buffer = uniformBuffers1[i];
        bufferInfo1.offset = 0;
        bufferInfo1.range = uniformBufferSize1;

        VkDescriptorBufferInfo bufferInfo2{};
        bufferInfo2.buffer = uniformBuffers2[i];
        bufferInfo2.offset = 0;
        bufferInfo2.range = uniformBufferSize2;

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = Zayn->texture_03_forTexture.textureImageView;
        imageInfo.sampler = Zayn->texture_03_forTexture.textureSampler;

        std::vector<VkWriteDescriptorSet> descriptorWrites{};

        VkWriteDescriptorSet descriptorWrite_uniform1{};
        descriptorWrite_uniform1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite_uniform1.dstSet = descriptorSets[i];
        descriptorWrite_uniform1.dstBinding = 0;
        descriptorWrite_uniform1.dstArrayElement = 0;
        descriptorWrite_uniform1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrite_uniform1.descriptorCount = 1;
        descriptorWrite_uniform1.pBufferInfo = &bufferInfo1;
        descriptorWrites.push_back(descriptorWrite_uniform1);

        VkWriteDescriptorSet descriptorWrite_uniform2{};
        descriptorWrite_uniform2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite_uniform2.dstSet = descriptorSets[i];
        descriptorWrite_uniform2.dstBinding = 1; // Assuming the second uniform buffer is bound to binding 1
        descriptorWrite_uniform2.dstArrayElement = 0;
        descriptorWrite_uniform2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrite_uniform2.descriptorCount = 1;
        descriptorWrite_uniform2.pBufferInfo = &bufferInfo2;
        descriptorWrites.push_back(descriptorWrite_uniform2);

        VkWriteDescriptorSet descriptorWrite_uniform3{};
        descriptorWrite_uniform3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite_uniform3.dstSet = descriptorSets[i];
        descriptorWrite_uniform3.dstBinding = 2; // Assuming the second uniform buffer is bound to binding 1
        descriptorWrite_uniform3.dstArrayElement = 0;
        descriptorWrite_uniform3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite_uniform3.descriptorCount = 1;
        descriptorWrite_uniform3.pImageInfo = &imageInfo;
        descriptorWrites.push_back(descriptorWrite_uniform3);


        vkUpdateDescriptorSets(Zayn->vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void CreateDescriptorSets_forLight(size_t uniformBufferSize1, std::vector<VkBuffer> &uniformBuffers1, VkDescriptorSetLayout *descriptorSetLayout, VkDescriptorPool *descriptorPool, std::vector<VkDescriptorSet> &descriptorSets)
{
    
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo1{};
        bufferInfo1.buffer = uniformBuffers1[i];
        bufferInfo1.offset = 0;
        bufferInfo1.range = uniformBufferSize1;

        std::vector<VkWriteDescriptorSet> descriptorWrites{};

        VkWriteDescriptorSet descriptorWrite_uniform1{};
        descriptorWrite_uniform1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite_uniform1.dstSet = descriptorSets[i];
        descriptorWrite_uniform1.dstBinding = 0;
        descriptorWrite_uniform1.dstArrayElement = 0;
        descriptorWrite_uniform1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrite_uniform1.descriptorCount = 1;
        descriptorWrite_uniform1.pBufferInfo = &bufferInfo1;
        descriptorWrites.push_back(descriptorWrite_uniform1);

        vkUpdateDescriptorSets(Zayn->vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void CreateDescriptorPool_forLight(VkDescriptorPool *descriptorPool)
{
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(Zayn->vkDevice, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void CreateDescriptorPool(VkDescriptorPool *descriptorPool)
{
    std::array<VkDescriptorPoolSize, 3> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(Zayn->vkDevice, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void InitRender()
{
    StartRender_Init();

    // LIGHTING_03_v3
    // 0. Add RenderComponent to Zayn
    CreatePushConstant<ModelPushConstant>(Zayn->vkPushConstantRanges_BASE_TYPE);
    CreateDescriptorSetLayout_03_Textures(&Zayn->vkDescriptorSetLayout_material_lighting_03_forTexture);


    CreateUniformBuffer(sizeof(UniformBufferObject_lighting_03), Zayn->uniformBuffers_materials_lighting_03_forTexture, Zayn->uniformBuffersMemory_materials_lighting_03_forTexture, Zayn->uniformBuffersMapped_materials_lighting_03_forTexture);
    CreateUniformBuffer(sizeof(MaterialBufferObject_lighting_03), Zayn->materialUniformBuffers_materials_lighting_03_forTexture, Zayn->materialUniformBuffersMemory_materials_lighting_03_forTexture, Zayn->materialUniformBuffersMapped_materials_lighting_03_forTexture);
   // CreateDescriptorSets_forLight(sizeof(UniformBufferObject_lighting_03), Zayn->uniformBuffers_materials_lighting_03_forTexture, &Zayn->vkDescriptorSetLayout_forLight, &Zayn->vkDescriptorPool_forLight, Zayn->vkDescriptorSets_material_lighting_03_forTexture);
    CreateGraphicsPipeline(Zayn, &Zayn->light_03_forTexture.pipeline, getShaderPath("vert_blank_light.spv"), getShaderPath("frag_blank_light.spv"), Zayn->vkPushConstantRanges_BASE_TYPE, &Zayn->vkDescriptorSetLayout_material_lighting_03_forTexture, &Zayn->vkPipelineLayout_material_lighting_03_forTexture);
    CreateGraphicsPipeline(Zayn, &Zayn->texture_03_forTexture.pipeline, getShaderPath("vert_blank_object_for_material_03_forTexture.spv"), getShaderPath("frag_blank_object_for_material_03_forTexture.spv"), Zayn->vkPushConstantRanges_BASE_TYPE, &Zayn->vkDescriptorSetLayout_material_lighting_03_forTexture, &Zayn->vkPipelineLayout_material_lighting_03_forTexture);

    CreateTextureImage(Zayn->texture_03_forTexture.mipLevels, &Zayn->texture_03_forTexture.textureImage, &Zayn->texture_03_forTexture.textureImageMemory,  getTexturePath( "viking_room.png"));
    CreateTextureImageView(Zayn->texture_03_forTexture.mipLevels, &Zayn->texture_03_forTexture.textureImage, &Zayn->texture_03_forTexture.textureImageView);
    CreateTextureSampler(Zayn->texture_03_forTexture.mipLevels, &Zayn->texture_03_forTexture.textureSampler);

    


    LoadModel(getModelPath("sphere.obj"), &Zayn->light_03_forTexture.vertices, &Zayn->light_03_forTexture.indices, glm::vec3(1.0f, 0.4f, 0.4f));
    LoadModel(getModelPath("gwc.obj"), &Zayn->texture_03_forTexture.vertices, &Zayn->texture_03_forTexture.indices, glm::vec3(1.0f,1.0f,1.0f));

    CreateVertexBuffer(Zayn->light_03_forTexture.vertices, &Zayn->light_03_forTexture.vertexBuffer, &Zayn->light_03_forTexture.vertexBufferMemory);
    CreateVertexBuffer(Zayn->texture_03_forTexture.vertices, &Zayn->texture_03_forTexture.vertexBuffer, &Zayn->texture_03_forTexture.vertexBufferMemory);

    CreateIndexBuffer(Zayn->light_03_forTexture.indices, &Zayn->light_03_forTexture.indexBuffer, &Zayn->light_03_forTexture.indexBufferMemory);
    CreateIndexBuffer(Zayn->texture_03_forTexture.indices, &Zayn->texture_03_forTexture.indexBuffer, &Zayn->texture_03_forTexture.indexBufferMemory);
   

    // CreateDescriptorSetLayout_forLight(&Zayn->vkDescriptorSetLayout_forLight);

    CreateDescriptorPool(&Zayn->vkDescriptorPool_material_lighting_03_forTexture); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    // CreateDescriptorPool_forLight(&Zayn->vkDescriptorPool_forLight); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    AllocateDescriptorSets(&Zayn->vkDescriptorSetLayout_material_lighting_03_forTexture, &Zayn->vkDescriptorPool_material_lighting_03_forTexture, Zayn->vkDescriptorSets_material_lighting_03_forTexture);
    // AllocateDescriptorSets(&Zayn->vkDescriptorSetLayout_forLight, &Zayn->vkDescriptorPool_forLight, Zayn->vkDescriptorSets_material_lighting_03_forTexture);
    
    CreateDescriptorSets_forTexture(sizeof(UniformBufferObject_lighting_03), sizeof(MaterialBufferObject_lighting_03), Zayn->uniformBuffers_materials_lighting_03_forTexture, Zayn->materialUniformBuffers_materials_lighting_03_forTexture, &Zayn->vkDescriptorSetLayout_material_lighting_03_forTexture, &Zayn->vkDescriptorPool_material_lighting_03_forTexture, Zayn->vkDescriptorSets_material_lighting_03_forTexture);
 

    // LIGHTING_01
    // 0. Add RenderComponent to Zayn
    // 1. Create descriptor set layout
    // CreateDescriptorSetLayout(&Zayn->vkDescriptorSetLayout_lighting_01, false);]
    // CreateUniformBuffer(sizeof(UniformBufferObject_lighting_03), Zayn->uniformBuffers_materials_lighting_03, Zayn->uniformBuffersMemory_materials_lighting_03, Zayn->uniformBuffersMapped_materials_lighting_03);
    // CreateUniformBuffer(sizeof(MaterialBufferObject_lighting_03), Zayn->materialUniformBuffers_materials_lighting_03, Zayn->materialUniformBuffersMemory_materials_lighting_03, Zayn->materialUniformBuffersMapped_materials_lighting_03);
    
    // CreateDescriptorSetLayout_L03(&Zayn->vkDescriptorSetLayout_material_lighting_03);
    // CreateDescriptorPool(&Zayn->vkDescriptorPool_material_lighting_03); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    // AllocateDescriptorSets(&Zayn->vkDescriptorSetLayout_material_lighting_03, &Zayn->vkDescriptorPool_material_lighting_03, Zayn->vkDescriptorSets_material_lighting_03);
    // CreateDescriptorSets(sizeof(UniformBufferObject_lighting_03), sizeof(MaterialBufferObject_lighting_03), Zayn->uniformBuffers_materials_lighting_03, Zayn->materialUniformBuffers_materials_lighting_03, &Zayn->vkDescriptorSetLayout_material_lighting_03, &Zayn->vkDescriptorPool_material_lighting_03, Zayn->vkDescriptorSets_material_lighting_03);
    
    // 2. Create Push Constant Range
    // CreatePushConstant<ModelPushConstant>(Zayn->vkPushConstantRanges_material_lighting_03);
    // 3. Create Graphics Pipeline
    // CreateGraphicsPipeline(Zayn, &Zayn->lighting_03.pipeline, getShaderPath("vert_blank_light.spv"), getShaderPath("frag_blank_light.spv"), Zayn->vkPushConstantRanges_material_lighting_03, &Zayn->vkDescriptorSetLayout_material_lighting_03, &Zayn->vkPipelineLayout_material_lighting_03);
    // CreateGraphicsPipeline(Zayn, &Zayn->material_03.pipeline, getShaderPath("vert_blank_object_for_material_03.spv"), getShaderPath("frag_blank_object_for_material_03.spv"), Zayn->vkPushConstantRanges_material_lighting_03, &Zayn->vkDescriptorSetLayout_material_lighting_03, &Zayn->vkPipelineLayout_material_lighting_03);
    // 4. Load Model
    // LoadModel(getModelPath("sphere.obj"), &Zayn->lighting_03.vertices, &Zayn->lighting_03.indices, glm::vec3(1.0f, 0.4f, 0.4f));
    // LoadModel(getModelPath("sphere.obj"), &Zayn->material_03.vertices, &Zayn->material_03.indices, glm::vec3(0.3f, 1.0f, 0.1f));
    // 5. Create Vertex Buffer
    // CreateVertexBuffer(Zayn->lighting_03.vertices, &Zayn->lighting_03.vertexBuffer, &Zayn->lighting_03.vertexBufferMemory);
    // CreateVertexBuffer(Zayn->material_03.vertices, &Zayn->material_03.vertexBuffer, &Zayn->material_03.vertexBufferMemory);
    // 6. Create Index Buffer
    // CreateIndexBuffer(Zayn->lighting_03.indices, &Zayn->lighting_03.indexBuffer, &Zayn->lighting_03.indexBufferMemory);
    // CreateIndexBuffer(Zayn->material_03.indices, &Zayn->material_03.indexBuffer, &Zayn->material_03.indexBufferMemory);
    // 7. Create Uniform Buffer
    // CreateUniformBuffer(Zayn->uniformBuffers_lighting_01, Zayn->uniformBuffersMemory_lighting_01, Zayn->uniformBuffersMapped_lighting_01);
    // 8. Create Descriptor Pool
    // 9. Create Descriptor Sets

    // LIGHTING_01
    // 0. Add RenderComponent to Zayn
    // 1. Create descriptor set layout
    // CreateDescriptorSetLayout(&Zayn->vkDescriptorSetLayout_lighting_01, false);
    // 2. Create Push Constant Range
    // CreatePushConstant<ModelPushConstant>();
    // 3. Create Graphics Pipeline
    // 4. Load Model
    // LoadModel(getModelPath("sphere.obj"), &Zayn->lighting_01_lightSource.vertices, &Zayn->lighting_01_lightSource.indices, glm::vec3(1.0f, 0.1f, 0.0f));
    // // 5. Create Vertex Buffer
    // CreateVertexBuffer(Zayn->lighting_01_lightSource.vertices, &Zayn->lighting_01_lightSource.vertexBuffer, &Zayn->lighting_01_lightSource.vertexBufferMemory);
    // // 6. Create Index Buffer
    // CreateIndexBuffer(Zayn->lighting_01_lightSource.indices, &Zayn->lighting_01_lightSource.indexBuffer, &Zayn->lighting_01_lightSource.indexBufferMemory);
    // // 7. Create Uniform Buffer
    // // 8. Create Descriptor Pool
    // CreateDescriptorPool(&Zayn->vkDescriptorPool_lighting_01, false); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    // // 9. Create Descriptor Sets
    // CreateDescriptorSets(false, sizeof(UniformBufferObject_lighting_01), Zayn->uniformBuffers_lighting_01, &Zayn->vkDescriptorSetLayout_lighting_01, &Zayn->vkDescriptorPool_lighting_01, Zayn->vkDescriptorSets_lighting_01, nullptr, nullptr);

    EndRender_Init();
}

void UpdateUniformBuffer_LightingCasette_Material_03(uint32_t currentImage, ZaynMemory *zaynMem, vec3 lightColor, vec3 lightPos, vec3 viewPos)
{
    Camera *cam = &Zayn->camera;

    UniformBufferObject_lighting_03 ubo = {};
    
    cam->front.x = cosf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));
    cam->front.y = sinf(DegToRad(cam->pitch));
    cam->front.z = sinf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));

    glm::vec3 camPos = glm::vec3(cam->pos.x, cam->pos.y, cam->pos.z);
    glm::vec3 camFront = glm::vec3(cam->front.x, cam->front.y, cam->front.z);
    glm::vec3 camUp = glm::vec3(cam->up.x, cam->up.y, cam->up.z);

    camFront = normalize(camFront);

    std::cout<< "cam Pos: ," << cam->pos.x << " " << cam->pos.y << " " << cam->pos.z << std::endl;

    ubo.view = glm::lookAt(camPos, camPos + camFront, camUp);
   
    ubo.proj = glm::perspective(glm::radians(60.0f), zaynMem->vkSwapChainExtent.width / (float)zaynMem->vkSwapChainExtent.height, 0.1f, 1000.0f);
    ubo.proj[1][1] *= -1;
    // varying light color 
    ubo.lightColor = lightColor;
    ubo.lightPos = lightPos;
    ubo.viewPos = cam->pos;

    memcpy(zaynMem->uniformBuffersMapped_materials_lighting_03_forTexture[currentImage], &ubo, sizeof(ubo));

    // Update the material buffer
    MaterialBufferObject_lighting_03 material = {};

    // change the material values over time

    float time = static_cast<float>(glfwGetTime());
    material.ambient   = V3(0.01f, 0.01f, 0.01f);
    material.diffuse   = V3(0.6f, 0.6f, 0.6f);
    material.specular  = V3(0.5f, 0.5f, 0.5f);
    material.shininess = 32.0f; 


    memcpy(zaynMem->materialUniformBuffersMapped_materials_lighting_03_forTexture[currentImage], &material, sizeof(material));



}


vec3 posModel1_lighting_01 = V3(0.0f, 10.0f, 0.0f);
vec3 posModel2_lighting_01 = V3(10.0f, 4.0f, 2.0f);


void UpdateRender_LightingCasette()
{
    if (BeginFrame())
    {
        // Vary light color by time
        vec3 lightColor = V3(1.0f, 1.0f, 1.0f);
        // move light back and forth in the z direction
        posModel2_lighting_01.x = 120.0f * cosf(glfwGetTime());

        // chnage rotation of the model over time
        // posModel1_lighting_01.y = 10.0f * cosf(glfwGetTime());
        
        UpdateUniformBuffer_LightingCasette_Material_03(Zayn->vkCurrentFrame, Zayn, lightColor, posModel2_lighting_01, Zayn->camera.pos);
        BeginSwapChainRenderPass(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame]);

        ModelPushConstant pushConstantData1 = {};
        ModelPushConstant pushConstantData2 = {};
        pushConstantData1.model_1 = TRS((posModel1_lighting_01), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(0.15f, 0.15f, 0.15f));
        pushConstantData2.model_1 = TRS((posModel2_lighting_01), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));

        // // RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets_lighting_01, &Zayn->lighting_01.vertexBuffer, &Zayn->lighting_01.indexBuffer, Zayn->lighting_01.indices, &pushConstantData1);
        // // RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->lighting_01_lightSource.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets_lighting_01, &Zayn->lighting_01_lightSource.vertexBuffer, &Zayn->lighting_01_lightSource.indexBuffer, Zayn->lighting_01_lightSource.indices, &pushConstantData2);

        RenderEntity_notYetEntity_DO(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->light_03_forTexture.pipeline, &Zayn->vkPipelineLayout_material_lighting_03_forTexture, Zayn->vkDescriptorSets_material_lighting_03_forTexture, &Zayn->light_03_forTexture.vertexBuffer, &Zayn->light_03_forTexture.indexBuffer, Zayn->light_03_forTexture.indices, &pushConstantData2);
        RenderEntity_notYetEntity_DO(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->texture_03_forTexture.pipeline, &Zayn->vkPipelineLayout_material_lighting_03_forTexture, Zayn->vkDescriptorSets_material_lighting_03_forTexture, &Zayn->texture_03_forTexture.vertexBuffer, &Zayn->texture_03_forTexture.indexBuffer, Zayn->texture_03_forTexture.indices, &pushConstantData1);


        #if IMGUI
        DoIMGUI();
        #endif

        EndSwapChainRenderPass(Zayn->vkCommandBuffers[Zayn->vkCurrentImageIndex]);
        EndFrame();
    }
}


void InitCasette()
{
    Zayn->myCasette = malloc(sizeof(MyCasette));
    memset(Zayn->myCasette, 0, sizeof(MyCasette));
    Casette = (MyCasette*)Zayn->myCasette;

    InitRender();

    InGameCameraInit();

}
void UpdateAndRenderCasette()
{
    Zayn->camera.frameCount += 1;

    // Define a base speed and an acceleration factor
    float baseSpeed = 0.0;
    float acceleration = 0.00000f;

    // Calculate the new speed
    float currentSpeed = baseSpeed + (acceleration * Zayn->camera.frameCount);

    InputCameraMovement();

    UpdateRender_LightingCasette();

}
