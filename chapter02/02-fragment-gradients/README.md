# Chapter 02-02: Fragment Gradients

## 학습 목표
- Fragment Shader에서 **다양한 그라데이션** 효과 구현
- **Fullscreen Quad** 렌더링 기법
- **Noise 함수**와 **FBM (Fractal Brownian Motion)** 이해

## 구현된 그라데이션 타입

### 1. Linear (선형)
- 특정 각도로 회전 가능한 선형 그라데이션
- `param1`으로 회전 각도 조절

### 2. Radial (방사형)
- 중심에서 바깥으로 퍼지는 원형 그라데이션
- `distance()` 함수 활용

### 3. Angular (각도)
- 중심을 기준으로 각도에 따른 그라데이션
- `atan()` 함수로 각도 계산

### 4. Diamond (다이아몬드)
- 마름모 형태의 그라데이션
- Manhattan distance 활용

### 5. Noise (노이즈)
- Perlin-like value noise 기반
- FBM으로 다층 노이즈 생성
- 시간에 따라 애니메이션

## 핵심 코드

### Fullscreen Quad (Vertex Shader)
```glsl
vec2 positions[6] = vec2[](
    vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(1.0, 1.0),
    vec2(-1.0, -1.0), vec2(1.0, 1.0), vec2(-1.0, 1.0)
);

void main() {
    vec2 pos = positions[gl_VertexIndex];
    gl_Position = vec4(pos, 0.0, 1.0);
    fragUV = pos * 0.5 + 0.5;  // [-1,1] -> [0,1]
}
```

### Value Noise (Fragment Shader)
```glsl
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);  // Smoothstep

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}
```

## ImGui 컨트롤
- **Gradient Type**: 그라데이션 종류 선택
- **Mix**: 색상 반전/혼합 정도
- **Param**: 회전 각도 등 추가 파라미터
- **Color 1/2**: 두 색상 선택
- **Animate Colors**: 자동 색상 애니메이션

## 빌드 및 실행

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/chapter02/02-fragment-gradients/ch02-02-fragment-gradients
```

## 학습 포인트

1. **Fullscreen Quad**
   - Vertex buffer 없이 삼각형 2개로 화면 전체 커버
   - UV 좌표를 통해 픽셀 위치 파악

2. **거리 함수**
   - Euclidean: `distance(a, b)` - 원형
   - Manhattan: `abs(a.x-b.x) + abs(a.y-b.y)` - 다이아몬드

3. **Procedural Noise**
   - Hash function으로 의사 난수 생성
   - Bilinear interpolation으로 부드러운 노이즈
   - FBM으로 자연스러운 패턴

## 파일 구조

```
02-fragment-gradients/
├── CMakeLists.txt
├── README.md
├── main.cpp
└── shaders/
    ├── gradient.vert
    ├── gradient.frag
    ├── vert.spv
    └── frag.spv
```
