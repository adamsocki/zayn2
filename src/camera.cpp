

void InputCameraMovement()
{
    Camera* cam = &Zayn->camera;

    if (InputHeld(Keyboard, Input_W))
    {
        cam->pos = cam->pos + cam->front * 15.0f * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_S))
    {
        cam->pos = cam->pos - cam->front * 15.0f * Zayn->deltaTime;
    }

    if (InputHeld(Keyboard, Input_D))
    {
        cam->pos = cam->pos + cam->right * 15.0f * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_A))
    {
        cam->pos = cam->pos - cam->right * 15.0f * Zayn->deltaTime;
    }

    if (InputHeld(Keyboard, Input_R))
    {
        cam->pos = cam->pos - cam->up * 15.0f * Zayn->deltaTime;
    }
    if (InputHeld(Keyboard, Input_F))
    {
        cam->pos = cam->pos - cam->up * 15.0f * Zayn->deltaTime;
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
}