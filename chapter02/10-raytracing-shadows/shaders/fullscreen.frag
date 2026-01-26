#version 450

// Fullscreen quad fragment shader

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D texSampler;

void main() {
    vec2 uv = vec2(inUV.x, 1.0 - inUV.y);  // Y flip
    outColor = texture(texSampler, uv);
}
