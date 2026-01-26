#version 450

layout(location = 0) in vec4 inColor;
layout(location = 1) in float inLifetime;

layout(location = 0) out vec4 outColor;

void main() {
    // Point sprite: gl_PointCoord is 0-1 within the point
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord) * 2.0;

    // Soft circle falloff
    float alpha = 1.0 - smoothstep(0.0, 1.0, dist);

    // Skip fragments outside circle
    if (alpha < 0.01) {
        discard;
    }

    // Apply particle color with fade
    outColor = vec4(inColor.rgb, inColor.a * alpha);

    // Add glow effect for young particles
    if (inLifetime > 1.5) {
        float glow = (inLifetime - 1.5) / 2.5;
        outColor.rgb += vec3(0.5, 0.3, 0.1) * glow * alpha;
    }
}
