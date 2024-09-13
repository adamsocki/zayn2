
void InGameCameraInit()
{
    
    Camera *cam = &Zayn->camera;
    cam->rotationSpeed = 100.0f;
    cam->targetPos = V3(0.0f, 0.0f, 0.0f);

    cam->currentSpeed = 0;
    cam->targetSpeed = 20.0f;
    cam->targetTurnSpeed = 160.0f;

    cam->pos = glm::vec3(0, 0, 0.5);
    cam->front = glm::vec3(-1, 0, 0);
    cam->up = glm::vec3(0, 0, 1);
    cam->right = glm::vec3(0, -1, 0);
}
