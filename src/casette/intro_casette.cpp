
#include <glm/gtc/matrix_transform.hpp>


void LoadShader()
{

}


void InitCasette()
{
    Zayn->myCasette = malloc(sizeof(MyCasette));
    memset(Zayn->myCasette, 0, sizeof(MyCasette));
    Casette = (MyCasette*)Zayn->myCasette;

    InitRender_Learn();

    AllocateEntityManager(&Casette->em, &Zayn->permanentMemArena, entityTypeCounts);
    

    InGameCameraInit();


    Casette_Entity casette = {};
    Zayn->casette_handle_1 = AddEntity(&Casette->em, Casette_Entity, &casette);
 
    Monkey firstMonkey = {};
    Monkey secondMonkey = {};
    Monkey thirdMonkey = {};
    Monkey fourthMonkey = {};
    Monkey fifthMonkey = {};

    firstMonkey.testingBool = true;

    Zayn->monkeyHandle1 = AddEntity(&Casette->em, Monkey, &firstMonkey);
    AddEntity(&Casette->em, Monkey, &secondMonkey);
    AddEntity(&Casette->em, Monkey, &thirdMonkey);
    AddEntity(&Casette->em, Monkey, &fourthMonkey);
    AddEntity(&Casette->em, Monkey, &fifthMonkey);

    // RenderComponent monkey_renderComponent = {};
    // monkey_renderComponent.modelPath = "/Users/socki/dev/zayn/models/viking_room.obj";
    // monkey_renderComponent.texturePath = "/Users/socki/dev/zayn/models/viking_room.obj";
    
    Monkey* testMonkey = GetEntity(&Casette->em, Monkey, Zayn->monkeyHandle1);
    testMonkey->model = &Zayn->model2;
    testMonkey->color = V3(0.1f, 0.2f, 0.6f);
    // testMonkey->transform2d.translation = V2(-0.4f, 0.0f);
    // mat2 sc = Scale2();
    // testMonkey->transform2d.scale = V2(3.4f, 0.12f);
    testMonkey->transform3d.translation = V3(0.0f, 0.0f, 0.53f);
    testMonkey->transform3d.scale = V3(1.0f, 1.0f, 1.0f);
    testMonkey->transform3d.rotation = V3(0.3f, 1.0f, 0.4f);
    testMonkey->transform3d.angleRotation = 5.0f;

    // testMonkey->renderComponent. 

    testMonkey->rad = 0.0f;
    testMonkey->posTest = 0.0f; 

    // zaynMem->camera.pos = V3(0.0f,0.0f,0.0f);
    // zaynMem->camera.viewMatrix = SetViewDirection(V3(0.0f, 0.0f, 0.0f), V3(0.5f, 0.0f, 1.0f), UP_V);
    // zaynMem->camera.viewMatrix = SetViewTarget(V3(0.0f, 0.0f, testMonkey->posTest), V3(0.0f, 0.0f, 0.53f), UP_V);

    Casette_Entity* cas = GetEntity(&Casette->em, Casette_Entity, Zayn->casette_handle_1);
    cas->model = &Zayn->casette_model;
    cas->transform3d.translation = V3(3.0f, 0.0f, 0.53f);
    cas->transform3d.scale = V3(1.0f, 1.0f, 1.0f);
    cas->transform3d.rotation = V3(0.3f, 1.0f, 0.4f);
    cas->transform3d.angleRotation = 5.0f;

    if (testMonkey)
    {
        std::cout << "testMonkey" << std::endl;
    }
    else
    {
        std::cout << "No testMonkey" << std::endl;
    }

}


void UpdateAndRenderCasette()
{

    // ********
	// LOGIC
	// ********
    
    
    // UpdateInput();

    
    // UpdateCamera();


    Monkey* testMonkey = GetEntity(&Casette->em, Monkey, Zayn->monkeyHandle1);
    // if(Inp)

    testMonkey->rad += 0.1f;
    testMonkey->rad += 0.1f;
    testMonkey->transform3d.angleRotation += 0.009f;
    testMonkey->posTest -= 0.01;

    // zaynMem->camera.pos.z -= 1.0f;

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



    UpdateRender_Learn();
    // DisplayMyImGui();

    // ********
	// RENDER
	// ********
    // std::cout << "hi" << std::endl;




    
}