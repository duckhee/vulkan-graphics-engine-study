#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    float explosionFactor;  // 0.0 = no explosion, 1.0+ = full explosion
    float time;
    float shrinkFactor;     // How much triangles shrink
    float rotationSpeed;
} ubo;

layout(location = 0) in vec3 inWorldPos[];
layout(location = 1) in vec3 inNormal[];
layout(location = 2) in vec3 inColor[];

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outColor;
layout(location = 3) out float outFade;

void main() {
    // Calculate triangle center
    vec3 center = (inWorldPos[0] + inWorldPos[1] + inWorldPos[2]) / 3.0;

    // Calculate face normal from edges
    vec3 edge1 = inWorldPos[1] - inWorldPos[0];
    vec3 edge2 = inWorldPos[2] - inWorldPos[0];
    vec3 faceNormal = normalize(cross(edge1, edge2));

    // Use average of input normals (should be same for flat shading)
    vec3 avgNormal = normalize(inNormal[0] + inNormal[1] + inNormal[2]);

    // Explosion displacement along face normal
    vec3 displacement = faceNormal * ubo.explosionFactor * 2.0;

    // Add some randomness based on center position
    float noise = sin(center.x * 10.0 + ubo.time) * cos(center.y * 10.0 + ubo.time) * 0.1;
    displacement += faceNormal * noise * ubo.explosionFactor;

    // Calculate rotation for spinning effect
    float spinAngle = ubo.time * ubo.rotationSpeed * (1.0 + length(center));
    mat3 spinMatrix = mat3(
        cos(spinAngle), -sin(spinAngle), 0.0,
        sin(spinAngle),  cos(spinAngle), 0.0,
        0.0,             0.0,            1.0
    );

    // Calculate shrink factor (triangles shrink as they explode)
    float shrink = 1.0 - ubo.shrinkFactor * ubo.explosionFactor;
    shrink = max(shrink, 0.1);  // Don't shrink to nothing

    // Calculate fade based on explosion factor
    float fade = 1.0 - ubo.explosionFactor * 0.5;
    fade = max(fade, 0.2);

    mat4 vp = ubo.proj * ubo.view;

    // Process each vertex of the triangle
    for (int i = 0; i < 3; i++) {
        // Move vertex relative to center
        vec3 relativePos = inWorldPos[i] - center;

        // Apply shrink
        relativePos *= shrink;

        // Apply spin (only when exploding)
        if (ubo.explosionFactor > 0.01) {
            relativePos = spinMatrix * relativePos;
        }

        // Move back to world space and apply displacement
        vec3 newPos = center + relativePos + displacement;

        // Transform to clip space
        gl_Position = vp * vec4(newPos, 1.0);

        // Pass interpolated data to fragment shader
        outWorldPos = newPos;
        outNormal = inNormal[i];  // Keep original normal for lighting
        outColor = inColor[i];
        outFade = fade;

        EmitVertex();
    }

    EndPrimitive();
}
