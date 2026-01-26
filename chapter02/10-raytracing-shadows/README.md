# 10-raytracing-shadows

## 개요
Compute Shader를 사용한 **소프트웨어 레이 트레이서** 구현입니다. 하드웨어 레이 트레이싱 지원 없이도 레이 트레이싱의 핵심 개념을 학습할 수 있습니다.

## 학습 목표
- 레이 트레이싱의 기본 원리 이해
- 광선-구(Ray-Sphere) 교차 알고리즘
- 광선-평면(Ray-Plane) 교차 알고리즘
- 그림자 광선(Shadow Rays)의 개념과 구현
- Phong 조명 모델 적용
- 반사(Reflections) 구현

## 핵심 개념

### 1. 레이 트레이싱 기본 원리
```
Camera → Ray → Scene Intersection → Shading → Pixel Color
   │
   └─→ For each pixel, cast a ray and find closest intersection
```

### 2. 광선-구 교차 (Ray-Sphere Intersection)
```
Ray: P(t) = Origin + t * Direction
Sphere: |P - Center|² = Radius²

Substituting:
|Origin + t*Dir - Center|² = R²

Quadratic equation: at² + bt + c = 0
where:
  a = Dir · Dir
  b = 2 * (Origin - Center) · Dir
  c = |Origin - Center|² - R²

discriminant = b² - 4ac
  > 0: Two intersections
  = 0: One intersection (tangent)
  < 0: No intersection
```

### 3. 광선-평면 교차 (Ray-Plane Intersection)
```
Plane: (P - Point) · Normal = 0
Ray: P(t) = Origin + t * Direction

t = (Point - Origin) · Normal / (Direction · Normal)
```

### 4. 그림자 광선 (Shadow Rays)
```
Hit Point → Shadow Ray → Light Source
              │
              └─→ If blocked by object: in shadow
                  If reaches light: illuminated
```

### 5. Phong 조명 모델
```
Color = Ambient + Shadow * (Diffuse + Specular)

Ambient  = ka * baseColor
Diffuse  = kd * max(N·L, 0) * baseColor * intensity
Specular = ks * pow(max(N·H, 0), shininess) * intensity

where:
  N = surface normal
  L = light direction
  H = half vector = normalize(L + V)
  V = view direction
```

### 6. 반사 (Reflections)
```
Reflect Direction = I - 2 * (N·I) * N

where:
  I = incident ray direction
  N = surface normal
```

## 장면 구성

### 오브젝트
1. **메인 구** (사용자 제어 가능)
   - 바운스 애니메이션
   - 색상, 위치, 크기 조절 가능

2. **추가 구들**
   - 녹색 구 (왼쪽)
   - 빨간 구 (오른쪽)

3. **체커보드 바닥**
   - 흰색/검은색 패턴
   - 그림자 표시

### 조명
- 점 광원 (Point Light)
- 위치 및 강도 조절 가능
- 궤도 애니메이션

## 컨트롤

### ImGui 패널
| 컨트롤 | 설명 |
|--------|------|
| Shadow Rays | 그림자 활성화/비활성화 |
| Shadow Intensity | 그림자 어두움 정도 (0.0 ~ 1.0) |
| Reflections | 반사 활성화/비활성화 |
| Camera Distance | 카메라 거리 |
| Camera Height | 카메라 높이 |
| Light X/Y/Z | 광원 위치 |
| Light Intensity | 광원 강도 |
| Sphere Radius | 메인 구 크기 |
| Sphere Color | 메인 구 색상 |
| Shininess | 반사 하이라이트 날카로움 |
| Animate Light | 광원 궤도 애니메이션 |

## 기술적 구현

### 파이프라인 구조
```
┌─────────────────────────────────────────┐
│           Compute Pipeline              │
│  ┌─────────────────────────────────┐    │
│  │   raytrace.comp                 │    │
│  │   - Per-pixel ray generation    │    │
│  │   - Scene intersection          │    │
│  │   - Shadow calculation          │    │
│  │   - Phong shading               │    │
│  │   - Reflection handling         │    │
│  │   → Write to Storage Image      │    │
│  └─────────────────────────────────┘    │
└─────────────────────────────────────────┘
                    │
                    ▼
         [Image Layout Transition]
         GENERAL → SHADER_READ_ONLY
                    │
                    ▼
┌─────────────────────────────────────────┐
│           Graphics Pipeline             │
│  ┌─────────────────────────────────┐    │
│  │   fullscreen.vert               │    │
│  │   - Generate fullscreen triangle│    │
│  └─────────────────────────────────┘    │
│  ┌─────────────────────────────────┐    │
│  │   fullscreen.frag               │    │
│  │   - Sample ray traced image     │    │
│  │   - Output to swapchain         │    │
│  └─────────────────────────────────┘    │
└─────────────────────────────────────────┘
```

### Shader 파일
| 파일 | 설명 |
|------|------|
| `raytrace.comp` | 메인 레이 트레이싱 compute shader |
| `fullscreen.vert` | 풀스크린 삼각형 vertex shader |
| `fullscreen.frag` | 텍스처 샘플링 fragment shader |

### Push Constants (Compute)
```cpp
struct PushConstants {
    vec4 cameraPos;       // xyz = position, w = fov
    vec4 lightPos;        // xyz = position, w = intensity
    vec4 spherePos;       // xyz = position, w = radius
    vec4 sphereColor;     // rgb = color, a = shininess
    float time;           // animation time
    float shadowIntensity;// shadow darkness
    int showShadows;      // enable shadows
    int reflections;      // enable reflections
};
```

## 빌드 및 실행

### 셰이더 컴파일
```bash
cd shaders
glslangValidator -V raytrace.comp -o raytrace_comp.spv
glslangValidator -V fullscreen.vert -o fullscreen_vert.spv
glslangValidator -V fullscreen.frag -o fullscreen_frag.spv
```

### 빌드
```bash
# 전체 프로젝트에서
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build

# 실행
./build/chapter02/10-raytracing-shadows/ch02-10
```

### 독립 빌드
```bash
cd chapter02/10-raytracing-shadows
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/ch02-10
```

## 성능 고려사항

### Compute Shader 설정
- **Work Group Size**: 16x16 (픽셀 블록)
- **Dispatch Size**: (width/16, height/16, 1)
- **Image Resolution**: 1024x768 (레이 트레이싱 해상도)

### 최적화
1. **Early Ray Termination**: 배경 히트 시 추가 계산 생략
2. **Shadow Bias**: 자기 교차 방지 (0.01 오프셋)
3. **단일 반사**: 무한 재귀 방지 (1회 반사만)

## 소프트웨어 vs 하드웨어 레이 트레이싱

| 항목 | 소프트웨어 (이 예제) | 하드웨어 RT |
|------|---------------------|-------------|
| 하드웨어 요구 | 모든 Vulkan GPU | RTX/RDNA2+ |
| 구현 복잡도 | 낮음 | 높음 |
| 성능 | 제한적 | 실시간 |
| 가속 구조 | 없음 | BVH/BLAS/TLAS |
| 학습 가치 | 원리 이해 | 실무 활용 |

## 확장 아이디어
- 다중 반사 (Recursive Reflections)
- 굴절 (Refraction)
- 소프트 섀도우 (Soft Shadows)
- 안티앨리어싱 (Supersampling)
- 추가 기하 도형 (박스, 삼각형)

## 참고 자료
- [Ray Tracing in One Weekend](https://raytracing.github.io/)
- [Scratchapixel - Ray-Sphere Intersection](https://www.scratchapixel.com/)
- [Real-Time Rendering - Shadow Algorithms](https://www.realtimerendering.com/)
