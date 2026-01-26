#version 450

// Vertex attributes (hardcoded cube)
vec3 positions[36] = vec3[](
    // Front face
    vec3(-0.5, -0.5,  0.5), vec3( 0.5, -0.5,  0.5), vec3( 0.5,  0.5,  0.5),
    vec3(-0.5, -0.5,  0.5), vec3( 0.5,  0.5,  0.5), vec3(-0.5,  0.5,  0.5),
    // Back face
    vec3( 0.5, -0.5, -0.5), vec3(-0.5, -0.5, -0.5), vec3(-0.5,  0.5, -0.5),
    vec3( 0.5, -0.5, -0.5), vec3(-0.5,  0.5, -0.5), vec3( 0.5,  0.5, -0.5),
    // Top face
    vec3(-0.5,  0.5,  0.5), vec3( 0.5,  0.5,  0.5), vec3( 0.5,  0.5, -0.5),
    vec3(-0.5,  0.5,  0.5), vec3( 0.5,  0.5, -0.5), vec3(-0.5,  0.5, -0.5),
    // Bottom face
    vec3(-0.5, -0.5, -0.5), vec3( 0.5, -0.5, -0.5), vec3( 0.5, -0.5,  0.5),
    vec3(-0.5, -0.5, -0.5), vec3( 0.5, -0.5,  0.5), vec3(-0.5, -0.5,  0.5),
    // Right face
    vec3( 0.5, -0.5,  0.5), vec3( 0.5, -0.5, -0.5), vec3( 0.5,  0.5, -0.5),
    vec3( 0.5, -0.5,  0.5), vec3( 0.5,  0.5, -0.5), vec3( 0.5,  0.5,  0.5),
    // Left face
    vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5,  0.5), vec3(-0.5,  0.5,  0.5),
    vec3(-0.5, -0.5, -0.5), vec3(-0.5,  0.5,  0.5), vec3(-0.5,  0.5, -0.5)
);

vec3 normals[36] = vec3[](
    // Front face
    vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 1.0),
    // Back face
    vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0),
    vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 0.0, -1.0),
    // Top face
    vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0),
    vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0),
    // Bottom face
    vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0),
    vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, -1.0, 0.0),
    // Right face
    vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0),
    vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0),
    // Left face
    vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0),
    vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0)
);

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

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;

void main()
{
    vec3 pos = positions[gl_VertexIndex];
    vec3 normal = normals[gl_VertexIndex];

    vec4 worldPos = ubo.model * vec4(pos, 1.0);
    fragPos = worldPos.xyz;
    fragNormal = mat3(transpose(inverse(ubo.model))) * normal;

    gl_Position = ubo.proj * ubo.view * worldPos;
}
