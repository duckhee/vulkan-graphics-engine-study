# 06-geometry-explosion

## 학습 목표
- **Geometry Shader**로 프리미티브 변형
- 삼각형 중심 및 면 노말 계산
- 복합 변환 (이동 + 회전 + 스케일)
- 시간 기반 애니메이션 효과

## 구현 내용

### 폭발 효과 원리
```
원본 삼각형 → Geometry Shader → 변형된 삼각형
                    ↓
         1. 삼각형 중심점 계산
         2. 면 노말 방향으로 이동
         3. 중심 기준 축소
         4. 스핀 회전 적용
```

### 핵심 코드: Geometry Shader

```glsl
#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {
    // 1. 삼각형 중심 계산
    vec3 center = (inWorldPos[0] + inWorldPos[1] + inWorldPos[2]) / 3.0;

    // 2. 면 노말 계산 (에지 외적)
    vec3 edge1 = inWorldPos[1] - inWorldPos[0];
    vec3 edge2 = inWorldPos[2] - inWorldPos[0];
    vec3 faceNormal = normalize(cross(edge1, edge2));

    // 3. 폭발 변위 (노말 방향으로)
    vec3 displacement = faceNormal * ubo.explosionFactor * 2.0;

    // 4. 축소 계산
    float shrink = 1.0 - ubo.shrinkFactor * ubo.explosionFactor;

    // 5. 스핀 회전
    float spinAngle = ubo.time * ubo.rotationSpeed;
    mat3 spinMatrix = /* 회전 행렬 */;

    // 각 정점 변환 및 출력
    for (int i = 0; i < 3; i++) {
        vec3 relativePos = inWorldPos[i] - center;
        relativePos *= shrink;
        relativePos = spinMatrix * relativePos;
        vec3 newPos = center + relativePos + displacement;

        gl_Position = vp * vec4(newPos, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
```

### UBO 구조체

```cpp
struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
    alignas(4) float explosionFactor;  // 폭발 정도 (0.0 ~ 2.0)
    alignas(4) float time;             // 애니메이션 시간
    alignas(4) float shrinkFactor;     // 축소 정도 (0.0 ~ 1.0)
    alignas(4) float rotationSpeed;    // 회전 속도
};
```

### 폭발 파라미터 효과

| 파라미터 | 효과 |
|----------|------|
| `explosionFactor = 0.0` | 원본 큐브 상태 |
| `explosionFactor = 0.5` | 면들이 약간 분리 |
| `explosionFactor = 1.0` | 완전 폭발 상태 |
| `explosionFactor = 2.0` | 더 멀리 분리 |
| `shrinkFactor = 0.0` | 삼각형 크기 유지 |
| `shrinkFactor = 1.0` | 삼각형 점으로 축소 |

## 파일 구조

```
06-geometry-explosion/
├── main.cpp                    # 전체 Vulkan 애플리케이션
├── CMakeLists.txt
├── README.md
└── shaders/
    ├── explosion.vert         # Vertex Shader (정점 데이터 준비)
    ├── explosion.geom         # Geometry Shader (폭발 변형)
    ├── explosion.frag         # Fragment Shader (조명 + 페이드)
    ├── explosion_vert.spv
    ├── explosion_geom.spv
    └── explosion_frag.spv
```

## 셰이더 컴파일

```bash
cd shaders
glslangValidator -V explosion.vert -o explosion_vert.spv
glslangValidator -V explosion.geom -o explosion_geom.spv
glslangValidator -V explosion.frag -o explosion_frag.spv
```

## 빌드 및 실행

```bash
# 루트 디렉토리에서
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build --target chapter0206GeometryExplosion

# 실행
./build/chapter02/06-geometry-explosion/ch02-06
```

## UI 컨트롤

| 컨트롤 | 설명 |
|--------|------|
| Explosion | 폭발 정도 슬라이더 (0.0 ~ 2.0) |
| Shrink Factor | 삼각형 축소 비율 (0.0 ~ 1.0) |
| Rotation Speed | 회전 속도 (0.0 ~ 10.0) |
| Auto Explode | 자동 폭발 애니메이션 |
| Auto Rotate | 큐브 자동 회전 |
| Reset | 모든 값 초기화 |

## 렌더링 특징

### 컬링 비활성화
```cpp
rasterizer.cullMode = VK_CULL_MODE_NONE;  // 폭발 시 뒷면도 표시
```

### Fresnel 엣지 글로우
```glsl
// Fragment Shader에서
float rim = 1.0 - max(dot(normal, viewDir), 0.0);
rim = pow(rim, 2.0) * 0.3;
result += vec3(1.0, 0.5, 0.2) * rim;  // 주황색 글로우
```

### 페이드 효과
```glsl
float fade = 1.0 - ubo.explosionFactor * 0.5;
result *= fade;  // 폭발할수록 어두워짐
```

## 예상 결과

- 화면 중앙에 회전하는 색상 큐브
- 슬라이더로 폭발 정도 조절
- Auto Explode로 자동 애니메이션
- 각 면이 노말 방향으로 분리되며 회전
- 엣지에 주황색 글로우 효과

## Geometry Shader 폭발 응용

| 응용 | 설명 |
|------|------|
| 게임 폭발 | 오브젝트 파괴 효과 |
| UI 트랜지션 | 화면 전환 효과 |
| 물리 시뮬레이션 | 충격파 표현 |
| 파티클 시스템 | 초기 버스트 효과 |

## 주의사항

1. **성능**: Geometry Shader는 GPU에서 비교적 느림
2. **정점 수**: max_vertices 제한 주의
3. **호환성**: 일부 GPU에서 미지원
4. **대안**: Compute Shader + GPU 파티클이 더 효율적

## 다음 단계

- [07-compute-particles](../07-compute-particles/): Compute Pipeline으로 파티클 시스템 구현
