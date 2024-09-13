cmake_minimum_required(VERSION 3.10)

# Name the project and specify the C++ standard
project(MyProject VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Toolchain file")

# Specify the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

# Include directories
if(WIN32)
    include_directories(C:/VulkanSDK/1.3.283.0/Include)
    include_directories(include)
else()
    include_directories(/usr/local/include include)
endif()

# Add the source files
set(SOURCES src/main.cpp)

# Add ImGui source files
set(IMGUI_SRC
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/backends/imgui_impl_vulkan.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
)

# Add the executable target
add_executable(main_pc ${SOURCES} ${IMGUI_SRC})

# Link against GLFW, Vulkan, and OpenGL
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(main_pc PRIVATE glfw Vulkan::Vulkan)

# Specify linker options (e.g., library paths)
if(NOT WIN32)
    target_link_options(main_pc PRIVATE
        -L/usr/local/lib
        -Wl,-rpath,/usr/local/lib
    )
endif()

# Additional compiler options
if(NOT WIN32)
    target_compile_options(main_pc PRIVATE 
        -fcolor-diagnostics 
        -fansi-escape-codes 
        -g
    )
endif()

# Include directories for ImGui
target_include_directories(main_pc PRIVATE
    external/imgui
    external/imgui/backends
)
