#version 450

// Tessellation terrain fragment shader
// 높이 기반 색상 + 조명

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in float inHeight;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    mat4 mvp;
    float tessLevelOuter;
    float tessLevelInner;
    float heightScale;
    float time;
} pc;

// 높이 기반 지형 색상
vec3 getTerrainColor(float height, vec3 normal) {
    // 높이에 따른 색상 결정
    vec3 water = vec3(0.1, 0.3, 0.6);      // 물 (파랑)
    vec3 sand = vec3(0.76, 0.70, 0.50);     // 모래 (노랑)
    vec3 grass = vec3(0.2, 0.5, 0.1);       // 풀 (초록)
    vec3 rock = vec3(0.5, 0.45, 0.4);       // 바위 (회색)
    vec3 snow = vec3(0.95, 0.95, 1.0);      // 눈 (흰색)

    vec3 color;

    if (height < 0.2) {
        // 물 ~ 모래
        color = mix(water, sand, smoothstep(0.1, 0.2, height));
    } else if (height < 0.4) {
        // 모래 ~ 풀
        color = mix(sand, grass, smoothstep(0.2, 0.4, height));
    } else if (height < 0.7) {
        // 풀 ~ 바위
        color = mix(grass, rock, smoothstep(0.4, 0.7, height));
    } else {
        // 바위 ~ 눈
        color = mix(rock, snow, smoothstep(0.7, 0.9, height));
    }

    // 경사도에 따른 바위 추가 (급경사는 바위)
    float slope = 1.0 - normal.y;
    if (slope > 0.5 && height > 0.3) {
        color = mix(color, rock, smoothstep(0.5, 0.8, slope));
    }

    return color;
}

void main() {
    // 노말 정규화
    vec3 normal = normalize(inNormal);

    // 조명 계산
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));  // 태양 방향
    vec3 viewDir = normalize(-inWorldPos);

    // Ambient
    float ambient = 0.3;

    // Diffuse
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular (물에만 적용)
    float specular = 0.0;
    if (inHeight < 0.15) {
        vec3 halfDir = normalize(lightDir + viewDir);
        specular = pow(max(dot(normal, halfDir), 0.0), 64.0) * 0.5;
    }

    // 지형 색상
    vec3 terrainColor = getTerrainColor(inHeight, normal);

    // 안개 효과 (거리 기반)
    float dist = length(inWorldPos);
    float fogFactor = exp(-dist * 0.02);
    vec3 fogColor = vec3(0.7, 0.8, 0.9);

    // 최종 색상 계산
    vec3 litColor = terrainColor * (ambient + diffuse * 0.7) + vec3(specular);
    vec3 finalColor = mix(fogColor, litColor, fogFactor);

    // 와이어프레임 힌트 (테셀레이션 시각화용 옵션)
    // 그리드 라인 표시
    vec2 grid = fract(inTexCoord * 16.0);
    float line = step(0.95, max(grid.x, grid.y));

    outColor = vec4(finalColor, 1.0);
}
