#version 450

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in float inFade;

layout(location = 0) out vec4 outColor;

// Simple lighting
const vec3 lightDir = normalize(vec3(0.5, 0.8, 0.6));
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float ambientStrength = 0.3;

void main() {
    vec3 normal = normalize(inNormal);

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine lighting with color
    vec3 result = (ambient + diffuse) * inColor;

    // Apply fade
    result *= inFade;

    // Add slight glow at edges (Fresnel-like effect)
    vec3 viewDir = normalize(-inWorldPos);
    float rim = 1.0 - max(dot(normal, viewDir), 0.0);
    rim = pow(rim, 2.0) * 0.3;
    result += vec3(1.0, 0.5, 0.2) * rim * (1.0 - inFade + 0.5);

    outColor = vec4(result, 1.0);
}
