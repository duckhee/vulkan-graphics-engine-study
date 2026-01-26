#version 450

// Fullscreen quad fragment shader
// 필터링된 이미지를 화면에 표시

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform sampler2D texSampler;

void main() {
    // Flip Y coordinate (Vulkan's coordinate system)
    vec2 uv = vec2(inUV.x, 1.0 - inUV.y);
    outColor = texture(texSampler, uv);
}
