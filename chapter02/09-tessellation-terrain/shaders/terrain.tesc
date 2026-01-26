#version 450

// Tessellation Control Shader
// 테셀레이션 레벨 및 패치 제어점 관리

layout(vertices = 4) out;  // quad patch

layout(location = 0) in vec3 inPosition[];
layout(location = 1) in vec2 inTexCoord[];

layout(location = 0) out vec3 outPosition[];
layout(location = 1) out vec2 outTexCoord[];

// Push constant로 테셀레이션 파라미터 전달
layout(push_constant) uniform PushConstants {
    mat4 mvp;
    float tessLevelOuter;
    float tessLevelInner;
    float heightScale;
    float time;
} pc;

void main() {
    // 현재 제어점 데이터 전달
    outPosition[gl_InvocationID] = inPosition[gl_InvocationID];
    outTexCoord[gl_InvocationID] = inTexCoord[gl_InvocationID];

    // 테셀레이션 레벨 설정 (첫 번째 호출에서만)
    if (gl_InvocationID == 0) {
        // Outer tessellation levels (각 엣지에 대한 분할 수)
        gl_TessLevelOuter[0] = pc.tessLevelOuter;
        gl_TessLevelOuter[1] = pc.tessLevelOuter;
        gl_TessLevelOuter[2] = pc.tessLevelOuter;
        gl_TessLevelOuter[3] = pc.tessLevelOuter;

        // Inner tessellation levels (내부 분할)
        gl_TessLevelInner[0] = pc.tessLevelInner;
        gl_TessLevelInner[1] = pc.tessLevelInner;
    }
}
