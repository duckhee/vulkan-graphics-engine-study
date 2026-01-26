#version 450

// Cube positions - 36 vertices (6 faces * 2 triangles * 3 vertices)
vec3 positions[36] = vec3[](
    // Front face (+Z)
    vec3(-0.5, -0.5,  0.5),
    vec3( 0.5, -0.5,  0.5),
    vec3( 0.5,  0.5,  0.5),
    vec3(-0.5, -0.5,  0.5),
    vec3( 0.5,  0.5,  0.5),
    vec3(-0.5,  0.5,  0.5),
    // Back face (-Z)
    vec3( 0.5, -0.5, -0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(-0.5,  0.5, -0.5),
    vec3( 0.5, -0.5, -0.5),
    vec3(-0.5,  0.5, -0.5),
    vec3( 0.5,  0.5, -0.5),
    // Top face (+Y)
    vec3(-0.5,  0.5,  0.5),
    vec3( 0.5,  0.5,  0.5),
    vec3( 0.5,  0.5, -0.5),
    vec3(-0.5,  0.5,  0.5),
    vec3( 0.5,  0.5, -0.5),
    vec3(-0.5,  0.5, -0.5),
    // Bottom face (-Y)
    vec3(-0.5, -0.5, -0.5),
    vec3( 0.5, -0.5, -0.5),
    vec3( 0.5, -0.5,  0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3( 0.5, -0.5,  0.5),
    vec3(-0.5, -0.5,  0.5),
    // Right face (+X)
    vec3( 0.5, -0.5,  0.5),
    vec3( 0.5, -0.5, -0.5),
    vec3( 0.5,  0.5, -0.5),
    vec3( 0.5, -0.5,  0.5),
    vec3( 0.5,  0.5, -0.5),
    vec3( 0.5,  0.5,  0.5),
    // Left face (-X)
    vec3(-0.5, -0.5, -0.5),
    vec3(-0.5, -0.5,  0.5),
    vec3(-0.5,  0.5,  0.5),
    vec3(-0.5, -0.5, -0.5),
    vec3(-0.5,  0.5,  0.5),
    vec3(-0.5,  0.5, -0.5)
);

// Normals for each vertex (per-face)
vec3 normals[36] = vec3[](
    // Front face (+Z)
    vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0),
    // Back face (-Z)
    vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0),
    vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0),
    // Top face (+Y)
    vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0),
    // Bottom face (-Y)
    vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0),
    vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0),
    // Right face (+X)
    vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0),
    vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0),
    // Left face (-X)
    vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0),
    vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0)
);

// Colors for each face
vec3 faceColors[6] = vec3[](
    vec3(1.0, 0.3, 0.3),  // Front - Red
    vec3(0.3, 1.0, 0.3),  // Back - Green
    vec3(0.3, 0.3, 1.0),  // Top - Blue
    vec3(1.0, 1.0, 0.3),  // Bottom - Yellow
    vec3(1.0, 0.3, 1.0),  // Right - Magenta
    vec3(0.3, 1.0, 1.0)   // Left - Cyan
);

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    float explosionFactor;  // 0.0 = no explosion, 1.0+ = full explosion
    float time;
    float shrinkFactor;     // How much triangles shrink (0 = no shrink, 1 = disappear)
    float rotationSpeed;
} ubo;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outColor;

void main() {
    vec3 pos = positions[gl_VertexIndex];
    vec3 normal = normals[gl_VertexIndex];

    // Determine which face (for color)
    int faceIndex = gl_VertexIndex / 6;
    vec3 color = faceColors[faceIndex];

    // Transform position to world space
    vec4 worldPos = ubo.model * vec4(pos, 1.0);

    // Transform normal to world space (using normal matrix)
    mat3 normalMatrix = transpose(inverse(mat3(ubo.model)));
    vec3 worldNormal = normalize(normalMatrix * normal);

    outWorldPos = worldPos.xyz;
    outNormal = worldNormal;
    outColor = color;

    // Final position will be computed in geometry shader
    gl_Position = worldPos;  // Pass world position to geometry shader
}
