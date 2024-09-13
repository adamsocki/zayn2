
#include "hash.cpp"

#include "zayn.h"

#include "model.cpp"

#include "entity.cpp"
#include "memory.cpp"
#include "input.cpp"
#include "camera_manager.cpp"

#include "renderer/render.cpp"

#include "my_imgui.cpp"

#include "casette/casette_system.cpp"

#include <GLFW/glfw3.h>


void ZaynInit(ZaynMemory *zaynMem)
{
    Zayn = zaynMem;
    Input = &zaynMem->inputManager;

    // uint64_t startTime = mach_absolute_time();

    InitCasette();

	Zayn->renderComponents = MakeDynamicArray<RenderComponent>(&Zayn->permanentMemArena, 150);
}


void ZaynUpdateAndRender(ZaynMemory *zaynMem)
{
    Zayn->accumTime += Zayn->deltaTime;
	Zayn->accumFrames++;
	if (Zayn->accumTime >= 1.0f)
	{
		float avgDt = Zayn->accumTime / Zayn->accumFrames;

		char windowName[64];
		snprintf(windowName, sizeof(windowName), "VkGalaxy [FPS: %.0f (%.2fms)]", 1.0f / avgDt, avgDt * 1000.0f);


#ifdef _WIN32
		SetWindowTextA((HWND)glfwGetWin32Window(Zayn->window), windowName);
#else
		glfwSetWindowTitle(Zayn->window, windowName);
#endif

		Zayn->accumTime -= 1.0f;
		Zayn->accumFrames = 0;
	}

	// TODO: create function to draw entity and draw instanced entities
#if IMGUI
   Render_MyIMGUI();
#endif

   UpdateAndRenderCasette();
}