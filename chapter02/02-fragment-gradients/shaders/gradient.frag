#version 450

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    float time;
    float gradientType;   // 0: linear, 1: radial, 2: angular, 3: diamond, 4: noise
    float colorMix;
    float param1;         // Rotation angle for linear/angular
    vec3 color1;
    float padding1;
    vec3 color2;
    float padding2;
} pc;

const float PI = 3.14159265359;

// Simple hash function for noise
float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

// Value noise
float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);

    // Smoothstep
    f = f * f * (3.0 - 2.0 * f);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// Fractal Brownian Motion (layered noise)
float fbm(vec2 p)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;

    for (int i = 0; i < 5; i++)
    {
        value += amplitude * noise(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    return value;
}

// Linear gradient
float linearGradient(vec2 uv, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);

    // Rotate UV
    vec2 rotated = vec2(
        uv.x * cosA - uv.y * sinA,
        uv.x * sinA + uv.y * cosA
    );

    return rotated.x;
}

// Radial gradient
float radialGradient(vec2 uv)
{
    vec2 center = vec2(0.5);
    float dist = distance(uv, center);
    return dist * 2.0;  // Scale to 0-1 range roughly
}

// Angular gradient
float angularGradient(vec2 uv, float offset)
{
    vec2 center = vec2(0.5);
    vec2 dir = uv - center;
    float angle = atan(dir.y, dir.x);
    return (angle + PI + offset) / (2.0 * PI);
}

// Diamond gradient
float diamondGradient(vec2 uv)
{
    vec2 center = vec2(0.5);
    vec2 diff = abs(uv - center);
    return (diff.x + diff.y) * 2.0;
}

// Noise-based gradient
float noiseGradient(vec2 uv, float time)
{
    return fbm(uv * 5.0 + vec2(time * 0.3, time * 0.2));
}

void main()
{
    float t = 0.0;
    int gradType = int(pc.gradientType);

    // Select gradient type
    if (gradType == 0)
    {
        // Linear gradient with rotation
        t = linearGradient(fragUV, pc.param1);
        t = clamp(t, 0.0, 1.0);
    }
    else if (gradType == 1)
    {
        // Radial gradient
        t = radialGradient(fragUV);
        t = clamp(t, 0.0, 1.0);
    }
    else if (gradType == 2)
    {
        // Angular gradient
        t = angularGradient(fragUV, pc.param1);
        t = fract(t);  // Wrap around
    }
    else if (gradType == 3)
    {
        // Diamond gradient
        t = diamondGradient(fragUV);
        t = clamp(t, 0.0, 1.0);
    }
    else // gradType == 4
    {
        // Noise gradient
        t = noiseGradient(fragUV, pc.time);
    }

    // Apply color mix factor
    t = mix(t, 1.0 - t, pc.colorMix);

    // Interpolate between colors
    vec3 finalColor = mix(pc.color1, pc.color2, t);

    // Add subtle animation
    float pulse = sin(pc.time * 2.0) * 0.05 + 1.0;
    finalColor *= pulse;

    outColor = vec4(finalColor, 1.0);
}
