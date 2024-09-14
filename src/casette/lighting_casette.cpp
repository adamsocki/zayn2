


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
    CreateGraphicsPipeline(Zayn, &Zayn->lighting_01.pipeline, getShaderPath("vert_blank_color.spv"), getShaderPath("frag_blank_color.spv"), Zayn->vkPushConstantRanges, &Zayn->vkDescriptorSetLayout_lighting_01, &Zayn->vkPipelineLayout);
    // 4. Load Model
    LoadModel(getModelPath("sphere.obj"), &Zayn->lighting_01.vertices, &Zayn->lighting_01.indices, glm::vec3(0.4f, 0.4f, 0.4f));
    // 5. Create Vertex Buffer
    CreateVertexBuffer(Zayn->lighting_01.vertices, &Zayn->lighting_01.vertexBuffer, &Zayn->lighting_01.vertexBufferMemory);
    // 6. Create Index Buffer
    CreateIndexBuffer(Zayn->lighting_01.indices, &Zayn->lighting_01.indexBuffer, &Zayn->lighting_01.indexBufferMemory);
    // 7. Create Uniform Buffer
    CreateUniformBuffer(Zayn->vkUniformBuffers, Zayn->vkUniformBuffersMemory, Zayn->vkUniformBuffersMapped);
    // 8. Create Descriptor Pool
    CreateDescriptorPool(&Zayn->vkDescriptorPool_lighting_01, false); // <---- CAN POTENTIAL BE RESUSED BETWEEN ENTITIES THAT HAVE THE SAME TYPES OF THINGS BEING SHARED
    // 9. Create Descriptor Sets
    CreateDescriptorSets(false, sizeof(UniformBufferObject), Zayn->vkUniformBuffers, &Zayn->vkDescriptorSetLayout_lighting_01, &Zayn->vkDescriptorPool_lighting_01, Zayn->vkDescriptorSets_lighting_01, nullptr, nullptr);
    


    EndRender_Init();
}

vec3 posModel1_lighting_01 = V3(0.0f, 0.0f, 0.0f);


void UpdateRender_LightingCasette()
{
    if (BeginFrame())
    {
        UpdateUniformBuffer(Zayn->vkCurrentFrame, Zayn);
        BeginSwapChainRenderPass(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame]);

        ModelPushConstant pushConstantData1 = {};
        pushConstantData1.model_1 = TRS((posModel1_lighting_01), AxisAngle(V3(0.0f, 0.2f, 0.20f), 0.0f), V3(1.0f, 1.0f, 1.0f));

        RenderEntity_notYetEntity(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame], &Zayn->lighting_01.pipeline, &Zayn->vkPipelineLayout, Zayn->vkDescriptorSets_lighting_01, &Zayn->lighting_01.vertexBuffer, &Zayn->lighting_01.indexBuffer, Zayn->lighting_01.indices, &pushConstantData1);


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

    Camera* cam = &Zayn->camera;

    if (InputHeld(Keyboard, Input_W))
    {
        cam->pos += cam->front * 15.0f * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_S))
    {
        cam->pos -= cam->front * 15.0f * Zayn->deltaTime;
    }

    if (InputHeld(Keyboard, Input_D))
    {
        cam->pos += cam->right * 15.0f * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_A))
    {
        cam->pos -= cam->right * 15.0f * Zayn->deltaTime;
    }

    if (InputHeld(Keyboard, Input_R))
    {
        cam->pos += cam->up * 15.0f * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_F))
    {
        cam->pos -= cam->up * 15.0f * Zayn->deltaTime;
    }

    // rotate camera aroudn the x y and z axis using inputheld and other keys
    if (InputHeld(Keyboard, Input_Q))
    {
        cam->yaw +=  cam->rotationSpeed * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_E))
    {
        cam->yaw -= cam->rotationSpeed * Zayn->deltaTime;
    }

    if (InputHeld(Keyboard, Input_T))
    {
        cam->pitch +=  cam->rotationSpeed * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_G))
    {
        cam->pitch -= cam->rotationSpeed * Zayn->deltaTime;
    }

    if (InputHeld(Keyboard, Input_Y))
    {
        cam->roll +=  cam->rotationSpeed * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_H))
    {
        cam->roll -= cam->rotationSpeed * Zayn->deltaTime;
    }

    UpdateRender_LightingCasette();

}
