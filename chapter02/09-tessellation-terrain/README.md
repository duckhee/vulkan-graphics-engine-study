# 09: Tessellation Terrain

## 개요
Vulkan Tessellation Shader를 사용하여 동적 LOD(Level of Detail) 지형을 생성합니다.

## 학습 목표
- **Tessellation Control Shader (TCS)** 이해
- **Tessellation Evaluation Shader (TES)** 이해
- **Patch Primitive** 사용법
- **Tessellation State** 설정
- **절차적 노이즈**로 지형 생성
- **와이어프레임 렌더링** 모드

## 테셀레이션 파이프라인

```
Vertex Shader → TCS → Tessellator → TES → Fragment Shader
      ↓          ↓        ↓           ↓
   정점 전달   레벨 설정  정점 생성   변환+높이맵
```

### 1. Tessellation Control Shader (TCS)
```glsl
layout(vertices = 4) out;  // Quad patch

void main() {
    // 테셀레이션 레벨 설정
    gl_TessLevelOuter[0] = tessLevel;
    gl_TessLevelOuter[1] = tessLevel;
    gl_TessLevelOuter[2] = tessLevel;
    gl_TessLevelOuter[3] = tessLevel;
    gl_TessLevelInner[0] = tessLevel;
    gl_TessLevelInner[1] = tessLevel;
}
```

### 2. Tessellation Evaluation Shader (TES)
```glsl
layout(quads, equal_spacing, ccw) in;

void main() {
    // gl_TessCoord를 사용해 보간
    vec3 pos = bilinearInterpolate(gl_TessCoord.xy);

    // 높이맵 적용
    pos.y = getHeight(pos.xz) * heightScale;

    gl_Position = mvp * vec4(pos, 1.0);
}
```

## 주요 개념

### Patch Primitive
```cpp
// Input Assembly에서 PATCH_LIST 지정
inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

// Tessellation State에서 제어점 개수 지정
tessellationState.patchControlPoints = 4;  // Quad
```

### Tessellation Level
| Level | 삼각형 수 (패치당) | 용도 |
|-------|-------------------|------|
| 1 | 2 | 최소 |
| 4 | ~32 | 저품질 |
| 16 | ~512 | 중간 품질 |
| 32 | ~2048 | 고품질 |
| 64 | ~8192 | 최대 품질 |

### 절차적 높이맵
```glsl
// FBM (Fractal Brownian Motion)
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(p);
        p *= 2.0;
        amplitude *= 0.5;
    }
    return value;
}
```

## 파일 구조
```
09-tessellation-terrain/
├── CMakeLists.txt       # 빌드 설정
├── README.md            # 이 파일
├── main.cpp             # 메인 구현 (~1280줄)
└── shaders/
    ├── terrain.vert     # Vertex shader (pass-through)
    ├── terrain.tesc     # Tessellation Control Shader
    ├── terrain.tese     # Tessellation Evaluation Shader
    ├── terrain.frag     # Fragment shader (높이 기반 색상)
    └── *.spv            # 컴파일된 셰이더들
```

## 빌드 방법

### Standalone 빌드
```bash
cd chapter02/09-tessellation-terrain
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build .
./bin/ch02-09
```

### 셰이더 컴파일
```bash
cd shaders
glslangValidator -V terrain.vert -o terrain_vert.spv
glslangValidator -V terrain.tesc -o terrain_tesc.spv
glslangValidator -V terrain.tese -o terrain_tese.spv
glslangValidator -V terrain.frag -o terrain_frag.spv
```

## ImGui 컨트롤

| 컨트롤 | 설명 |
|--------|------|
| Outer Level | 외곽 테셀레이션 레벨 (1-64) |
| Inner Level | 내부 테셀레이션 레벨 (1-64) |
| Height Scale | 지형 높이 배율 |
| Wireframe | 와이어프레임 모드 토글 |
| Auto Rotate | 카메라 자동 회전 |

## 키보드 컨트롤

| 키 | 기능 |
|----|------|
| W/S | 카메라 전/후 이동 |
| A/D | 카메라 좌/우 이동 |
| Q/E | 카메라 상/하 이동 |
| Space | 와이어프레임 토글 |

## 핵심 구현

### 1. 피처 활성화
```cpp
VkPhysicalDeviceFeatures features{};
features.tessellationShader = VK_TRUE;  // 테셀레이션 필수
features.fillModeNonSolid = VK_TRUE;    // 와이어프레임용
```

### 2. 4-Stage 파이프라인
```cpp
std::array<VkPipelineShaderStageCreateInfo, 4> stages = {
    vertStage,   // Vertex
    tescStage,   // Tessellation Control
    teseStage,   // Tessellation Evaluation
    fragStage    // Fragment
};
```

### 3. Tessellation State
```cpp
VkPipelineTessellationStateCreateInfo tessState{};
tessState.patchControlPoints = 4;  // Quad patches
pipelineInfo.pTessellationState = &tessState;
```

### 4. 높이 기반 지형 색상
| 높이 | 색상 | 지형 |
|------|------|------|
| 0.0-0.2 | 파랑→노랑 | 물→모래 |
| 0.2-0.4 | 노랑→초록 | 모래→풀 |
| 0.4-0.7 | 초록→회색 | 풀→바위 |
| 0.7-1.0 | 회색→흰색 | 바위→눈 |

## 지형 구조

```
8x8 Grid = 64 Patches
각 Patch = 4 Control Points (Quad)
총 정점 = 256 (64 * 4)

테셀레이션 레벨 16일 때:
- 패치당 ~512 삼각형
- 총 ~32,768 삼각형
```

## 확장 아이디어

1. **Distance-Based LOD**: 카메라 거리에 따른 자동 LOD
2. **높이맵 텍스처**: 실제 지형 데이터 사용
3. **노말맵**: 디테일한 조명
4. **물 효과**: 반사/굴절
5. **Frustum Culling**: 패치 단위 컬링

## 관련 리소스

- [Vulkan Tessellation](https://www.khronos.org/opengl/wiki/Tessellation)
- [GPU Gems 2 - Terrain Rendering](https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-2-terrain-rendering-using-gpu-based-geometry)
- [FBM Noise](https://www.iquilezles.org/www/articles/fbm/fbm.htm)
