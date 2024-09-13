


void InitRender()
{
    StartRender_Init();

    // 1. Create descriptor set layout
    CreateDescriptorSetLayout(&Zayn->vkDescriptorSetLayout_Lighting, false);
    // 2. Create Push Constant Range
    CreatePushConstant<ModelPushConstant>();

    EndRender_Init();
}


void UpdateRender_LightingCasette()
{
    if (BeginFrame())
    {
        BeginSwapChainRenderPass(Zayn->vkCommandBuffers[Zayn->vkCurrentFrame]);







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

    UpdateRender_LightingCasette();

}
