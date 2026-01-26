#version 450

// Tessellation Evaluation Shader
// 테셀레이션된 정점 계산 및 높이맵 적용

layout(quads, equal_spacing, ccw) in;

layout(location = 0) in vec3 inPosition[];
layout(location = 1) in vec2 inTexCoord[];

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;
layout(location = 3) out float outHeight;

layout(push_constant) uniform PushConstants {
    mat4 mvp;
    float tessLevelOuter;
    float tessLevelInner;
    float heightScale;
    float time;
} pc;

// 절차적 노이즈 함수 (심플렉스 노이즈 근사)
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    // Cubic Hermite smoothing
    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(
        mix(hash(i + vec2(0.0, 0.0)), hash(i + vec2(1.0, 0.0)), u.x),
        mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), u.x),
        u.y
    );
}

// FBM (Fractal Brownian Motion) - 다중 옥타브 노이즈
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;

    // 4 옥타브
    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(p * frequency);
        frequency *= 2.0;
        amplitude *= 0.5;
    }

    return value;
}

// 높이 계산 함수
float getHeight(vec2 pos) {
    // 다양한 노이즈 조합으로 자연스러운 지형 생성
    float h = 0.0;

    // 기본 언덕
    h += fbm(pos * 0.5 + pc.time * 0.05) * 0.6;

    // 디테일 노이즈
    h += noise(pos * 2.0) * 0.2;

    // 산악 지형 효과
    float ridge = abs(noise(pos * 0.8 + vec2(100.0)) - 0.5) * 2.0;
    h += ridge * ridge * 0.4;

    return h;
}

// 노말 계산 (유한 차분법)
vec3 calculateNormal(vec3 pos) {
    float eps = 0.05;
    float hL = getHeight(pos.xz - vec2(eps, 0.0)) * pc.heightScale;
    float hR = getHeight(pos.xz + vec2(eps, 0.0)) * pc.heightScale;
    float hD = getHeight(pos.xz - vec2(0.0, eps)) * pc.heightScale;
    float hU = getHeight(pos.xz + vec2(0.0, eps)) * pc.heightScale;

    vec3 normal = normalize(vec3(hL - hR, 2.0 * eps, hD - hU));
    return normal;
}

void main() {
    // gl_TessCoord는 [0, 1] 범위의 파라미터화된 좌표
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // 4개의 제어점을 바이리니어 보간
    vec3 p0 = mix(inPosition[0], inPosition[1], u);
    vec3 p1 = mix(inPosition[3], inPosition[2], u);
    vec3 pos = mix(p0, p1, v);

    // 텍스처 좌표 보간
    vec2 t0 = mix(inTexCoord[0], inTexCoord[1], u);
    vec2 t1 = mix(inTexCoord[3], inTexCoord[2], u);
    vec2 texCoord = mix(t0, t1, v);

    // 높이맵 적용
    float height = getHeight(pos.xz);
    pos.y = height * pc.heightScale;

    // 월드 좌표와 노말 계산
    outWorldPos = pos;
    outNormal = calculateNormal(pos);
    outTexCoord = texCoord;
    outHeight = height;

    // 최종 클립 공간 좌표
    gl_Position = pc.mvp * vec4(pos, 1.0);
}
