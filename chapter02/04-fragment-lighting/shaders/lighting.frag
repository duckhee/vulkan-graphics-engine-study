#version 450

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

// Uniform Buffer Object
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 lightPos;      // w unused
    vec4 viewPos;       // w unused
    vec4 lightColor;    // w = ambient strength
    vec4 objectColor;   // w = specular strength
    float shininess;
    float padding1;
    float padding2;
    float padding3;
} ubo;

void main()
{
    vec3 lightPos = ubo.lightPos.xyz;
    vec3 viewPos = ubo.viewPos.xyz;
    vec3 lightColor = ubo.lightColor.rgb;
    vec3 objectColor = ubo.objectColor.rgb;
    float ambientStrength = ubo.lightColor.w;
    float specularStrength = ubo.objectColor.w;
    float shininess = ubo.shininess;

    // Normalize the normal
    vec3 norm = normalize(fragNormal);

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Final color
    vec3 result = (ambient + diffuse + specular) * objectColor;
    outColor = vec4(result, 1.0);
}
