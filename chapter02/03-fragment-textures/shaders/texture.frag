#version 450

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    float time;
    float textureType;
    float scale;
    float param1;
} pc;

const float PI = 3.14159265359;

// Checkerboard pattern
vec3 checkerboard(vec2 uv, float scale)
{
    vec2 pos = floor(uv * scale);
    float pattern = mod(pos.x + pos.y, 2.0);
    return mix(vec3(0.1, 0.1, 0.15), vec3(0.9, 0.9, 0.95), pattern);
}

// Circles pattern
vec3 circles(vec2 uv, float scale, float time)
{
    vec2 pos = fract(uv * scale) - 0.5;
    float dist = length(pos);
    float ring = smoothstep(0.3, 0.32, dist) - smoothstep(0.4, 0.42, dist);
    float center = 1.0 - smoothstep(0.0, 0.2, dist);

    vec3 color1 = vec3(0.2, 0.5, 0.8);
    vec3 color2 = vec3(0.8, 0.3, 0.2);
    float t = sin(time + length(floor(uv * scale)) * 0.5) * 0.5 + 0.5;

    return mix(color1, color2, t) * (ring + center * 0.5) + vec3(0.1);
}

// Stripes pattern
vec3 stripes(vec2 uv, float scale, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);
    vec2 rotated = vec2(uv.x * cosA - uv.y * sinA, uv.x * sinA + uv.y * cosA);

    float pattern = sin(rotated.x * scale * PI * 2.0) * 0.5 + 0.5;

    vec3 color1 = vec3(0.9, 0.7, 0.3);
    vec3 color2 = vec3(0.3, 0.2, 0.5);

    return mix(color1, color2, pattern);
}

// Procedural noise texture
float hash(vec2 p) { return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453); }

float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

vec3 procedural(vec2 uv, float scale, float time)
{
    float n1 = noise(uv * scale + time * 0.2);
    float n2 = noise(uv * scale * 2.0 - time * 0.3);
    float n3 = noise(uv * scale * 4.0 + time * 0.1);

    float combined = n1 * 0.5 + n2 * 0.3 + n3 * 0.2;

    vec3 color1 = vec3(0.1, 0.3, 0.6);
    vec3 color2 = vec3(0.8, 0.4, 0.2);
    vec3 color3 = vec3(0.2, 0.7, 0.4);

    vec3 result = mix(color1, color2, smoothstep(0.3, 0.5, combined));
    result = mix(result, color3, smoothstep(0.6, 0.8, combined));

    return result;
}

void main()
{
    int texType = int(pc.textureType);
    vec3 color;

    if (texType == 0)
        color = checkerboard(fragUV, pc.scale);
    else if (texType == 1)
        color = circles(fragUV, pc.scale, pc.time);
    else if (texType == 2)
        color = stripes(fragUV, pc.scale, pc.param1);
    else
        color = procedural(fragUV, pc.scale, pc.time);

    outColor = vec4(color, 1.0);
}
