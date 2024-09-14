


void InitRender()
{
    StartRender_Init();

    // LIGHTING_01
    // 0. Add RenderComponent to Zayn
    // 1. Create descriptor set layout
    CreateDescriptorSetLayout(&Zayn->vkDescriptorSetLayout_lighting_01, false);
    // 2. Create Push Constant Range
    CreatePushConstant<ModelPushConstant>();
    // 3. Create Graphics Pipeline
    CreateGraphicsPipeline(Zayn, &Zayn->lighting_01.pipeline, getShaderPath("vert_blank_object_for_light_02.spv"), getShaderPath("frag_blank_object_for_light_02.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout_lighting_01, &Zayn->vkPipelineLayout);
    // 4. Load Model
    LoadModel(getModelPath("sphere.obj"), &Zayn->lighting_01.vertices, &Zayn->lighting_01.indices, glm::vec3(1.0f, 0.4f, 0.4f));
    // 5. Create Vertex Buffer
    CreateVertexBuffer(Zayn->lighting_01.vertices, &Zayn->lighting_01.vertexBuffer, &Zayn->lighting_01.vertexBufferMemory);
    // 6. Create Index Buffer
    CreateIndexBuffer(Zayn->lighting_01.indices, &Zayn->lighting_01.indexBuffer, &Zayn->lighting_01.indexBufferMemory);
    // 7. Create Uniform Buffer
    CreateUniformBuffer(Zayn->uniformBuffers_lighting_01, Zayn->uniformBuffersMemory_lighting_01, Zayn->uniformBuffersMapped_lighting_01);
    // 8. Create Descriptor Pool
    CreateDescriptorPool(&Zayn->vkDescriptorPool_lighting_01, false); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    // 9. Create Descriptor Sets
    CreateDescriptorSets(false, sizeof(UniformBufferObject_lighting_01), Zayn->uniformBuffers_lighting_01, &Zayn->vkDescriptorSetLayout_lighting_01, &Zayn->vkDescriptorPool_lighting_01, Zayn->vkDescriptorSets_lighting_01, nullptr, nullptr);
    

    // LIGHTING_01
    // 0. Add RenderComponent to Zayn
    // 1. Create descriptor set layout
    CreateDescriptorSetLayout(&Zayn->vkDescriptorSetLayout_lighting_01, false);
    // 2. Create Push Constant Range
    CreatePushConstant<ModelPushConstant>();
    // 3. Create Graphics Pipeline
    CreateGraphicsPipeline(Zayn, &Zayn->lighting_01_lightSource.pipeline, getShaderPath("vert_blank_light.spv"), getShaderPath("frag_blank_light.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout_lighting_01, &Zayn->vkPipelineLayout);
    // 4. Load Model
    LoadModel(getModelPath("sphere.obj"), &Zayn->lighting_01_lightSource.vertices, &Zayn->lighting_01_lightSource.indices, glm::vec3(1.0f, 0.1f, 0.0f));
    // 5. Create Vertex Buffer
    CreateVertexBuffer(Zayn->lighting_01_lightSource.vertices, &Zayn->lighting_01_lightSource.vertexBuffer, &Zayn->lighting_01_lightSource.vertexBufferMemory);
    // 6. Create Index Buffer
    CreateIndexBuffer(Zayn->lighting_01_lightSource.indices, &Zayn->lighting_01_lightSource.indexBuffer, &Zayn->lighting_01_lightSource.indexBufferMemory);
    // 7. Create Uniform Buffer
    CreateUniformBuffer(Zayn->uniformBuffers_lighting_01, Zayn->uniformBuffersMemory_lighting_01, Zayn->uniformBuffersMapped_lighting_01);
    // 8. Create Descriptor Pool
    CreateDescriptorPool(&Zayn->vkDescriptorPool_lighting_01, false); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    // 9. Create Descriptor Sets
    CreateDescriptorSets(false, sizeof(UniformBufferObject_lighting_01), Zayn->uniformBuffers_lighting_01, &Zayn->vkDescriptorSetLayout_lighting_01, &Zayn->vkDescriptorPool_lighting_01, Zayn->vkDescriptorSets_lighting_01, nullptr, nullptr);
    


    EndRender_Init();
}

void UpdateUniformBuffer_LightingCasette_02(uint32_t currentImage, ZaynMemory *zaynMem, vec3 lightColor, vec3 lightPos, vec3 viewPos)
{
    Camera *cam = &Zayn->camera;

    UniformBufferObject_lighting_02 ubo = {};
    
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

    memcpy(zaynMem->uniformBuffersMapped_lighting_01[currentImage], &ubo, sizeof(ubo));
}


vec3 posModel1_lighting_01 = V3(0.0f, 0.0f, 0.0f);
vec3 posModel2_lighting_01 = V3(10.0f, 4.0f, 2.0f);


void UpdateRender_LightingCasette()
{
    if (BeginFrame())
    {
        // Vary light color by time
        vec3 lightColor = V3(0.0f, 1.0f, 1.0f);
        UpdateUniformBuffer_LightingCasette_02(Zayn->vkCurrentFrame, Zayn, lightColor, posModel2_lighting_01, Zayn->camera.pos);
        BeginSwapChainRenderPass(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame]);

        ModelPushConstant pushConstantData1 = {};
        ModelPushConstant pushConstantData2 = {};
        pushConstantData1.model_1 = TRS((posModel1_lighting_01), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));
        pushConstantData2.model_1 = TRS((posModel2_lighting_01), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));

        RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->lighting_01.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets_lighting_01, &Zayn->lighting_01.vertexBuffer, &Zayn->lighting_01.indexBuffer, Zayn->lighting_01.indices, &pushConstantData1);
        RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->lighting_01_lightSource.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets_lighting_01, &Zayn->lighting_01_lightSource.vertexBuffer, &Zayn->lighting_01_lightSource.indexBuffer, Zayn->lighting_01_lightSource.indices, &pushConstantData2);




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
