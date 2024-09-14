#version 450


layout(push_constant) uniform PushConstant {
    mat4 model_1;
} pc;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 lightingColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(lightingColor * fragColor, 1.0f);
}