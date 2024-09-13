#version 450

layout(push_constant) uniform PushConstant {
    mat4 model_1;
} pc;

layout(location = 1) in vec3 nearPoint; // nearPoint calculated in vertex shader
layout(location = 2) in vec3 farPoint; // farPoint calculated in vertex shader

layout(location = 0) out vec4 outColor;

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    outColor = vec4(1.0, 0.0, 0.0, 1.0 * float(t > 0)); // opacity = 1 when t > 0, opacity = 0 otherwise
}

