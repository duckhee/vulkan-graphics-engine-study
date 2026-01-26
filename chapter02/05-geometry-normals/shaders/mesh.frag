#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    float normalLength;
    float time;
    float showNormals;
    float padding;
} ubo;

void main()
{
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 normal = normalize(fragNormal);

    // Simple diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = vec3(0.2);
    vec3 diffuse = diff * vec3(0.8);

    // Color based on normal direction
    vec3 normalColor = normal * 0.5 + 0.5;  // Map [-1,1] to [0,1]

    vec3 color = (ambient + diffuse) * normalColor;
    outColor = vec4(color, 1.0);
}
