
#if OPENGL
#include "render_opengl.cpp"
#endif

#if VULKAN
#if ENGINE_2D
// #include "render_vulkan_2d.cpp"
#elif ENGINE_3D

#include "render_vulkan.cpp"



#endif
#endif


