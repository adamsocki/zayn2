
void InGameCameraInit()
{
    
    Camera *cam = &Zayn->camera;
    cam->rotationSpeed = 100.0f;
    cam->targetPos = V3(0.0f, 0.0f, 0.0f);

    cam->currentSpeed = 0;
    cam->targetSpeed = 20.0f;
    cam->targetTurnSpeed = 160.0f;

    cam->pos = V3(0, 0, 0.5f);
    cam->front = V3(-1, 0, 0);
    cam->up = V3(0, 0, 1);
    cam->right = V3(0, -1, 0);
}
