#version 450

// Tessellation terrain vertex shader
// 패치 제어점을 TCS에 전달

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec2 outTexCoord;

void main() {
    // 패치 정점을 그대로 전달 (실제 변환은 TES에서)
    outPosition = inPosition;
    outTexCoord = inTexCoord;
}
