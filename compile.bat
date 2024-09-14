dev_c\
set VULKAN_SDK=C:\VulkanSDK\1.3.283.0
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\frag_blank.frag -o C:\dev_c\zayn\src\renderer\shaders\frag_blank.spv
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\frag_blank_light.frag -o C:\dev_c\zayn\src\renderer\shaders\frag_blank_light.spv
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\vert_blank_light.vert -o C:\dev_c\zayn\src\renderer\shaders\vert_blank_light.spv
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\vert_blank_color.vert -o C:\dev_c\zayn\src\renderer\shaders\vert_blank_color.spv
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\vkShader_3d_INIT_vert.vert -o C:\dev_c\zayn\src\renderer\shaders\vkShader_3d_INIT_vert.spv
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\vkShader_3d_INIT_vert_inst.vert -o C:\dev_c\zayn\src\renderer\shaders\vkShader_3d_INIT_vert_inst.spv
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\grid_vert.vert -o C:\dev_c\zayn\src\renderer\shaders\grid_vert.spv
%VULKAN_SDK%\Bin\glslc.exe C:\dev_c\zayn\src\renderer\shaders\grid_frag.frag -o C:\dev_c\zayn\src\renderer\shaders\grid_frag.spv
pause
