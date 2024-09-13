#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    mat4 viewProj;
} ubo;


layout(push_constant) uniform PushConstant {
    mat4 model_1;
} pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
// out for fragment shader
// layout(location = 0) out vec3 fragColor;

layout(location = 1) out vec3 nearPoint;
layout(location = 2) out vec3 farPoint;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}



// Calculate the vertex position based on gl_VertexID
void main() {
    vec3 p = inPosition;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, ubo.view, ubo.proj);
    farPoint = UnprojectPoint(p.x, p.y, 1.0, ubo.view, ubo.proj);
    
    gl_Position = ubo.proj * ubo.view * pc.model_1 * vec4(p, 1.0);
}
