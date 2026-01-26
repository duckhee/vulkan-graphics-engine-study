# 05-geometry-normals

## 학습 목표
- **Geometry Shader** 파이프라인 스테이지 이해
- 정점에서 새로운 프리미티브 생성
- 멀티 파이프라인 렌더링 구현
- 노말 벡터 시각화 기법

## 구현 내용

### Geometry Shader 개념
```
Vertex Shader → [Geometry Shader] → Rasterizer → Fragment Shader
```

Geometry Shader는 Vertex Shader 이후에 실행되며:
- 입력된 프리미티브(점, 선, 삼각형)를 받아
- 새로운 프리미티브를 생성하거나 기존 것을 변형
- **max_vertices**만큼 정점 출력 가능

### 핵심 코드: Geometry Shader

```glsl
#version 450

// 입력: 삼각형 (3개의 정점)
layout(triangles) in;

// 출력: 선 스트립 (노말당 2개 정점, 총 6개)
layout(line_strip, max_vertices = 6) out;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    float normalLength;
    float time;
    float showNormals;
} ubo;

layout(location = 0) in vec3 inWorldPos[];
layout(location = 1) in vec3 inNormal[];

layout(location = 0) out vec3 outColor;

void main() {
    // showNormals가 0.5 미만이면 아무것도 출력하지 않음
    if (ubo.showNormals < 0.5) {
        return;
    }

    mat4 vp = ubo.proj * ubo.view;

    // 삼각형의 각 정점에서 노말 라인 생성
    for (int i = 0; i < 3; i++) {
        vec3 pos = inWorldPos[i];
        vec3 normal = normalize(inNormal[i]);

        // 시작점 (정점 위치) - 청록색
        gl_Position = vp * vec4(pos, 1.0);
        outColor = vec3(0.0, 1.0, 1.0);
        EmitVertex();

        // 끝점 (정점 + 노말 * 길이) - 노란색
        vec3 endPos = pos + normal * ubo.normalLength;
        gl_Position = vp * vec4(endPos, 1.0);
        outColor = vec3(1.0, 1.0, 0.0);
        EmitVertex();

        EndPrimitive();  // 하나의 선 완성
    }
}
```

### 두 개의 파이프라인

```cpp
// 1. 메시 렌더링 파이프라인
VkPipeline meshPipeline;          // Vertex + Fragment
VkPipelineLayout meshPipelineLayout;

// 2. 노말 시각화 파이프라인
VkPipeline normalsPipeline;       // Vertex + Geometry + Fragment
VkPipelineLayout normalsPipelineLayout;
```

### Geometry Shader 활성화

```cpp
// 1. Device Feature 요청
VkPhysicalDeviceFeatures deviceFeatures{};
deviceFeatures.geometryShader = VK_TRUE;

// 2. Device 생성 시 전달
VkDeviceCreateInfo createInfo{};
createInfo.pEnabledFeatures = &deviceFeatures;
```

### Descriptor 공유

두 파이프라인이 동일한 UBO를 사용:

```cpp
// Geometry Shader에서도 UBO 접근 가능
VkDescriptorSetLayoutBinding uboLayoutBinding{};
uboLayoutBinding.binding = 0;
uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT |
                               VK_SHADER_STAGE_GEOMETRY_BIT |
                               VK_SHADER_STAGE_FRAGMENT_BIT;
```

## 파일 구조

```
05-geometry-normals/
├── main.cpp                  # 듀얼 파이프라인 구현
├── CMakeLists.txt
├── README.md
└── shaders/
    ├── mesh.vert            # 메시용 Vertex Shader
    ├── mesh.frag            # 메시용 Fragment Shader
    ├── normals.vert         # 노말용 Vertex Shader
    ├── normals.geom         # 노말 생성 Geometry Shader
    ├── normals.frag         # 노말 라인 Fragment Shader
    ├── mesh_vert.spv        # 컴파일된 셰이더
    ├── mesh_frag.spv
    ├── normals_vert.spv
    ├── normals_geom.spv
    └── normals_frag.spv
```

## 셰이더 컴파일

```bash
cd shaders
glslangValidator -V mesh.vert -o mesh_vert.spv
glslangValidator -V mesh.frag -o mesh_frag.spv
glslangValidator -V normals.vert -o normals_vert.spv
glslangValidator -V normals.geom -o normals_geom.spv
glslangValidator -V normals.frag -o normals_frag.spv
```

## 빌드 및 실행

```bash
# 루트 디렉토리에서
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build --target chapter0205GeometryNormals

# 실행
./build/chapter02/05-geometry-normals/ch02-05
```

## UI 컨트롤

| 컨트롤 | 설명 |
|--------|------|
| Show Normals | 노말 벡터 표시 토글 |
| Normal Length | 노말 라인 길이 (0.01 ~ 0.5) |
| Auto Rotate | 큐브 자동 회전 |

## 렌더링 순서

```cpp
void drawFrame() {
    // 1. 메시 렌더링 (삼각형)
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, meshPipeline);
    vkCmdDraw(cmd, 36, 1, 0, 0);  // 큐브 36개 정점

    // 2. 노말 시각화 (라인)
    if (showNormals) {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, normalsPipeline);
        vkCmdDraw(cmd, 36, 1, 0, 0);  // 동일한 정점, Geometry Shader가 라인 생성
    }

    // 3. ImGui UI
    renderImGui();
}
```

## 예상 결과

- 화면 중앙에 회전하는 큐브
- 각 정점에서 노말 방향으로 라인 표시 (청록색 → 노란색)
- 노말 길이 실시간 조절 가능
- 면별로 노말 방향 확인 가능

## Geometry Shader 응용 분야

| 응용 | 설명 |
|------|------|
| 노말 시각화 | 디버깅용 노말 벡터 표시 |
| 폭발 효과 | 면을 바깥으로 이동 |
| 빌보드 | 점을 사각형으로 확장 |
| 와이어프레임 | 삼각형 엣지를 라인으로 |
| 털/잔디 | 정점에서 다수의 라인 생성 |
| 쉐도우 볼륨 | 실루엣 엣지 압출 |

## 주의사항

1. **성능**: Geometry Shader는 비교적 느림 (Mesh Shader 권장)
2. **호환성**: 일부 모바일 GPU에서 미지원
3. **출력 제한**: max_vertices 초과 불가
4. **드라이버**: macOS MoltenVK에서 제한적 지원

## 다음 단계

- [06-geometry-explosion](../06-geometry-explosion/): Geometry Shader로 폭발 효과 구현
