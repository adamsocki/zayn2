#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    vec3 lightColor;
    vec3 lightPos;
    vec3 viewPos;
} ubo;


layout(push_constant) uniform PushConstant {
    mat4 model_1;
} pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 texCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 objectColor;
layout(location = 1) out vec3 lightingColor;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 lightPos;
layout(location = 4) out vec3 fragPos;
layout(location = 5) out vec3 viewPos;

void main() {
    fragPos = vec3(pc.model_1 * vec4(inPosition, 1.0));
    gl_Position = ubo.proj * ubo.view * pc.model_1 * vec4(inPosition, 1.0);
    objectColor = inColor;
    lightingColor = ubo.lightColor;
    lightPos = ubo.lightPos;
    normal = mat3(transpose(inverse(pc.model_1))) * inNormal; // move this to the cpu when possible
    viewPos = ubo.viewPos;
}