#version 450

// Particle structure
struct Particle {
    vec4 position;   // xyz = position, w = lifetime
    vec4 velocity;   // xyz = velocity, w = mass
    vec4 color;      // rgba
};

// Storage buffer for particles (read-only in vertex shader)
layout(std430, binding = 0) readonly buffer ParticleBuffer {
    Particle particles[];
};

// Uniform buffer for rendering
layout(binding = 2) uniform RenderParams {
    mat4 view;
    mat4 proj;
    float pointSize;
    float time;
} render;

layout(location = 0) out vec4 outColor;
layout(location = 1) out float outLifetime;

void main() {
    Particle p = particles[gl_VertexIndex];

    // Transform position
    vec4 viewPos = render.view * vec4(p.position.xyz, 1.0);
    gl_Position = render.proj * viewPos;

    // Calculate point size (bigger when closer)
    float dist = length(viewPos.xyz);
    gl_PointSize = render.pointSize * (1.0 / dist) * 20.0;
    gl_PointSize = clamp(gl_PointSize, 1.0, 64.0);

    // Pass color and lifetime
    outColor = p.color;
    outLifetime = p.position.w;
}
