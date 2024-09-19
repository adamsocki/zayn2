#version 450

layout(push_constant) uniform PushConstant {
    mat4 model_1;
} pc;

layout(location = 0) in vec3 objectColor;
layout(location = 1) in vec3 lightingColor;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 lightPos;
layout(location = 4) in vec3 fragPos;
layout(location = 5) in vec3 viewPos;
layout(location = 6) in vec3 ambientVal;
layout(location = 7) in vec3 diffuseVal;
layout(location = 8) in vec3 specularVal;
layout(location = 9) in float shininessVal;
layout(location = 10) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 2) uniform sampler2D textureSampler;

void main() {
    // Sample the texture
    vec4 texColor = texture(textureSampler, fragTexCoord);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientVal * lightingColor;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightingColor * (diffuseVal * diff);

    // specular
    float specularStrength = 1.0; // Increase the specular strength
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float shininess = 64.0; // Increase the shininess factor
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessVal);
    vec3 specular = lightingColor * (specularVal * spec);

    // Combine the texture color with the lighting result
    vec3 result = (ambient + diffuse + specular) * objectColor * texColor.rgb;

    outColor = vec4(result, texColor.a);
}