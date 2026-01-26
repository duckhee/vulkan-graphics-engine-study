# 04-fragment-lighting: Phong/Blinn-Phong 라이팅

## 학습 목표

이 예제에서는 **Uniform Buffer Object(UBO)**를 사용하여 3D 큐브에 **Phong/Blinn-Phong 라이팅**을 구현합니다.
Push Constants보다 더 큰 데이터를 셰이더에 전달할 수 있는 방법을 배웁니다.

## 주요 개념

### 1. Uniform Buffer Object (UBO) vs Push Constants

| 특성 | Push Constants | UBO |
|------|----------------|-----|
| 최대 크기 | 128 바이트 (보통) | 64KB+ |
| 업데이트 속도 | 매우 빠름 | 빠름 |
| 사용 용도 | 작은 데이터 (시간, 타입) | 큰 데이터 (행렬, 조명) |
| Descriptor 필요 | 아니오 | 예 |

### 2. UBO 구조체 (std140 레이아웃)

```cpp
struct UniformBufferObject {
    alignas(16) glm::mat4 model;      // 모델 변환
    alignas(16) glm::mat4 view;       // 뷰 변환
    alignas(16) glm::mat4 proj;       // 투영 변환
    alignas(16) glm::vec4 lightPos;   // 광원 위치
    alignas(16) glm::vec4 viewPos;    // 카메라 위치
    alignas(16) glm::vec4 lightColor; // 광원 색상 (w = ambient)
    alignas(16) glm::vec4 objectColor;// 물체 색상 (w = specular)
    alignas(4)  float shininess;      // 반사광 선명도
};
```

### 3. Phong 라이팅 모델

**세 가지 구성 요소**:
- **Ambient**: 주변광 (기본 밝기)
- **Diffuse**: 난반사 (물체 방향 의존)
- **Specular**: 정반사 (시점 의존, Blinn-Phong 사용)

```glsl
// Fragment Shader
vec3 ambient = ambientStrength * lightColor;

vec3 lightDir = normalize(lightPos - fragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

vec3 viewDir = normalize(viewPos - fragPos);
vec3 halfwayDir = normalize(lightDir + viewDir);  // Blinn-Phong
float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
vec3 specular = specularStrength * spec * lightColor;

vec3 result = (ambient + diffuse + specular) * objectColor;
```

### 4. Descriptor System

UBO를 셰이더에 바인딩하기 위한 구성요소:
1. **DescriptorSetLayout**: UBO 바인딩 정의
2. **DescriptorPool**: Descriptor 할당 풀
3. **DescriptorSet**: 실제 리소스 바인딩

## 새로운 기능

### Depth Buffer
- 3D 렌더링에서 Z-버퍼링 필요
- `VK_FORMAT_D32_SFLOAT` 포맷 사용
- Framebuffer에 depth attachment 추가

### 3D 큐브 렌더링
- 36개의 정점 (6면 × 2삼각형 × 3정점)
- 각 면의 노말 벡터 포함

## 빌드 및 실행

```bash
# 프로젝트 루트에서
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build --target chapter0204FragmentLighting

# 실행
./build/chapter02/ch02-04-fragment-lighting
```

## 컨트롤

| 컨트롤 | 설명 |
|--------|------|
| Auto Rotate Light | 광원 자동 회전 |
| Light Position | 광원 위치 (수동) |
| Light Color | 광원 색상 |
| Object Color | 물체 색상 |
| Ambient | 주변광 강도 |
| Specular | 반사광 강도 |
| Shininess | 반사광 선명도 (2-256) |
| Auto Rotate Cube | 큐브 자동 회전 |

## 파일 구조

```
04-fragment-lighting/
├── CMakeLists.txt
├── README.md
├── main.cpp              # UBO + Descriptor 설정
└── shaders/
    ├── lighting.vert     # MVP 변환 + 노말 전달
    ├── lighting.frag     # Phong 라이팅 계산
    ├── vert.spv
    └── frag.spv
```

## 셰이더 컴파일

```bash
cd chapter02/04-fragment-lighting/shaders
glslangValidator -V lighting.vert -o vert.spv
glslangValidator -V lighting.frag -o frag.spv
```

## 학습 포인트

1. **UBO 생성 및 매핑**: Host-visible 버퍼 생성, 영구 매핑
2. **Descriptor 시스템**: Layout → Pool → Set 생성 흐름
3. **std140 레이아웃**: `alignas()` 매크로로 정렬 보장
4. **Depth 테스트**: 3D 렌더링에 필수
5. **Blinn-Phong**: halfway 벡터로 더 자연스러운 반사

## 다음 단계

- **05-geometry-normals**: Geometry Shader를 사용한 노말 벡터 시각화
