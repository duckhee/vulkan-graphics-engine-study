#version 450

// Fullscreen quad vertex shader
// 화면 전체를 덮는 삼각형 2개로 쿼드 생성

layout(location = 0) out vec2 outUV;

void main() {
    // Generate fullscreen triangle (covers screen with 3 vertices)
    // Vertex 0: (-1, -1), Vertex 1: (3, -1), Vertex 2: (-1, 3)
    outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    gl_Position = vec4(outUV * 2.0 - 1.0, 0.0, 1.0);
}
