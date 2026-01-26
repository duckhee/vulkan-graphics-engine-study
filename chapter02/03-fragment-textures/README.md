# 03-fragment-textures: 절차적 텍스처 생성

## 학습 목표

이 예제에서는 **Push Constants**를 사용하여 Fragment Shader에서 **절차적 텍스처**(Procedural Textures)를 생성합니다.
실제 이미지 파일 없이 수학적 함수만으로 다양한 패턴을 만들어냅니다.

## 주요 개념

### 1. 절차적 텍스처란?
- 이미지 파일 대신 **수학적 알고리즘**으로 생성되는 텍스처
- 무한한 해상도, 적은 메모리 사용
- 실시간 파라미터 조절 가능

### 2. Push Constants 구조
```cpp
struct PushConstants {
    float time;         // 애니메이션 시간
    float textureType;  // 텍스처 종류 (0-3)
    float scale;        // 패턴 스케일
    float param1;       // 추가 파라미터
};
```

### 3. 구현된 텍스처 패턴

| 타입 | 이름 | 설명 |
|------|------|------|
| 0 | Checkerboard | 클래식한 체커보드 패턴 |
| 1 | Circles | 시간에 따라 색상이 변하는 원형 패턴 |
| 2 | Stripes | 회전 가능한 줄무늬 패턴 |
| 3 | Procedural | 다중 옥타브 노이즈 기반 자연스러운 패턴 |

## 셰이더 기술

### Checkerboard
```glsl
vec3 checkerboard(vec2 uv, float scale) {
    vec2 pos = floor(uv * scale);
    float pattern = mod(pos.x + pos.y, 2.0);
    return mix(color1, color2, pattern);
}
```

### Noise Function (Hash + Value Noise)
```glsl
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);  // Smoothstep interpolation

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}
```

## 빌드 및 실행

```bash
# 프로젝트 루트에서
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build --target chapter0203FragmentTextures

# 실행
./build/chapter02/ch02-03-fragment-textures
```

## 컨트롤

| 컨트롤 | 설명 |
|--------|------|
| Texture Type | 텍스처 패턴 선택 (Checkerboard/Circles/Stripes/Procedural) |
| Scale | 패턴의 밀도/크기 조절 |
| Angle | Stripes 패턴의 회전 각도 |

## 파일 구조

```
03-fragment-textures/
├── CMakeLists.txt
├── README.md
├── main.cpp              # Vulkan 애플리케이션 + ImGui
└── shaders/
    ├── texture.vert      # 버텍스 셰이더 (fullscreen quad)
    ├── texture.frag      # 프래그먼트 셰이더 (절차적 텍스처)
    ├── vert.spv          # 컴파일된 버텍스 셰이더
    └── frag.spv          # 컴파일된 프래그먼트 셰이더
```

## 셰이더 컴파일

```bash
cd chapter02/03-fragment-textures/shaders
glslangValidator -V texture.vert -o vert.spv
glslangValidator -V texture.frag -o frag.spv
```

## 학습 포인트

1. **절차적 생성의 장점**: 메모리 효율적, 무한 해상도
2. **노이즈 함수**: 자연스러운 패턴의 기초
3. **UV 좌표 조작**: floor, fract를 사용한 타일링
4. **시간 기반 애니메이션**: sin/cos으로 부드러운 변화

## 다음 단계

- **04-fragment-lighting**: Uniform Buffer Object(UBO)를 사용한 Phong 라이팅 구현
