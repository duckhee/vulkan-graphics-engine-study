#version 450

// Push constants from CPU
layout(push_constant) uniform PushConstants {
    float time;
    float animationType;  // 0: wave, 1: rotation, 2: scale pulse, 3: spiral
    float amplitude;
    float frequency;
} pc;

// Hardcoded triangle vertices
vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.2, 0.3),  // Red-ish
    vec3(0.2, 1.0, 0.3),  // Green-ish
    vec3(0.2, 0.3, 1.0)   // Blue-ish
);

layout(location = 0) out vec3 fragColor;

const float PI = 3.14159265359;

// Wave animation - vertices move up and down in a wave pattern
vec2 waveAnimation(vec2 pos, int vertexIndex)
{
    float phase = float(vertexIndex) * 2.0 * PI / 3.0;
    float offset = sin(pc.time * pc.frequency + phase) * pc.amplitude;
    return vec2(pos.x, pos.y + offset);
}

// Rotation animation - triangle rotates around center
vec2 rotationAnimation(vec2 pos)
{
    float angle = pc.time * pc.frequency;
    float cosA = cos(angle);
    float sinA = sin(angle);

    // Rotation matrix
    return vec2(
        pos.x * cosA - pos.y * sinA,
        pos.x * sinA + pos.y * cosA
    );
}

// Scale pulse animation - triangle pulses in size
vec2 scalePulseAnimation(vec2 pos)
{
    float scale = 1.0 + sin(pc.time * pc.frequency) * pc.amplitude;
    return pos * scale;
}

// Spiral animation - vertices spiral outward and inward
vec2 spiralAnimation(vec2 pos, int vertexIndex)
{
    float phase = float(vertexIndex) * 2.0 * PI / 3.0;
    float angle = pc.time * pc.frequency + phase;
    float radius = length(pos);
    float newRadius = radius + sin(pc.time * pc.frequency * 0.5) * pc.amplitude * 0.5;

    // Rotate and scale
    float cosA = cos(angle);
    float sinA = sin(angle);

    vec2 rotated = vec2(
        pos.x * cosA - pos.y * sinA,
        pos.x * sinA + pos.y * cosA
    );

    return rotated * (newRadius / radius);
}

void main()
{
    vec2 pos = positions[gl_VertexIndex];
    vec2 animatedPos;

    // Select animation based on type
    int animType = int(pc.animationType);

    if (animType == 0)
    {
        animatedPos = waveAnimation(pos, gl_VertexIndex);
    }
    else if (animType == 1)
    {
        animatedPos = rotationAnimation(pos);
    }
    else if (animType == 2)
    {
        animatedPos = scalePulseAnimation(pos);
    }
    else // animType == 3
    {
        animatedPos = spiralAnimation(pos, gl_VertexIndex);
    }

    gl_Position = vec4(animatedPos, 0.0, 1.0);

    // Animate colors slightly based on time
    vec3 baseColor = colors[gl_VertexIndex];
    float colorShift = sin(pc.time * 0.5) * 0.1 + 0.1;
    fragColor = baseColor + vec3(colorShift);
}
