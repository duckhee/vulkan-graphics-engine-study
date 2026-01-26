# Chapter 02-01: Vertex Animation

## 학습 목표
- **Push Constants** 사용법 이해
- Vertex Shader에서 **시간 기반 애니메이션** 구현
- 다양한 애니메이션 타입 구현 및 전환

## 구현된 기능

### 애니메이션 타입
1. **Wave (파도)**: 각 vertex가 시간에 따라 상하로 움직임
2. **Rotation (회전)**: 삼각형이 중심을 기준으로 회전
3. **Scale Pulse (크기 펄스)**: 삼각형이 커졌다 작아졌다 반복
4. **Spiral (나선)**: vertex들이 나선형으로 회전하며 크기 변화

### Push Constants
```glsl
layout(push_constant) uniform PushConstants {
    float time;           // 현재 시간
    float animationType;  // 0: wave, 1: rotation, 2: scale, 3: spiral
    float amplitude;      // 애니메이션 진폭
    float frequency;      // 애니메이션 주파수
} pc;
```

### ImGui 컨트롤
- Animation Type: 애니메이션 종류 선택
- Amplitude: 움직임 크기 조절 (0.0 ~ 1.0)
- Frequency: 속도 조절 (0.1 ~ 5.0)
- Pause/Resume: 애니메이션 일시정지
- Reset: 시간 초기화

## 핵심 코드

### Push Constants 설정 (C++)
```cpp
// Pipeline layout에 push constant 범위 추가
VkPushConstantRange pushConstantRange{};
pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
pushConstantRange.offset = 0;
pushConstantRange.size = sizeof(PushConstants);

// Command buffer에서 push constants 업데이트
vkCmdPushConstants(commandBuffer, pipelineLayout,
    VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);
```

### Vertex Shader 애니메이션 예시 (Wave)
```glsl
vec2 waveAnimation(vec2 pos, int vertexIndex) {
    float phase = float(vertexIndex) * 2.0 * PI / 3.0;
    float offset = sin(pc.time * pc.frequency + phase) * pc.amplitude;
    return vec2(pos.x, pos.y + offset);
}
```

## 빌드 및 실행

```bash
# 프로젝트 루트에서
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build

# 실행
./build/chapter02/01-vertex-animation/ch02-01-vertex-animation
```

## Shader 수정 시

```bash
cd chapter02/01-vertex-animation/shaders
glslangValidator -V animation.vert -o vert.spv
glslangValidator -V animation.frag -o frag.spv
```

## 학습 포인트

1. **Push Constants vs Uniform Buffers**
   - Push Constants: 작은 데이터 (< 128 bytes), 매 프레임 변경되는 데이터
   - UBO: 큰 데이터, 덜 자주 변경되는 데이터

2. **Shader에서의 시간 활용**
   - `sin(time)`, `cos(time)`: 주기적 움직임
   - `time * frequency`: 속도 조절
   - `phase offset`: 각 vertex별 다른 타이밍

3. **2D 변환**
   - 회전: 회전 행렬 적용
   - 스케일: 위치에 스칼라 곱
   - 이동: 위치에 오프셋 더하기

## 파일 구조

```
01-vertex-animation/
├── CMakeLists.txt
├── README.md
├── main.cpp
└── shaders/
    ├── animation.vert    # Vertex shader 소스
    ├── animation.frag    # Fragment shader 소스
    ├── vert.spv          # 컴파일된 vertex shader
    └── frag.spv          # 컴파일된 fragment shader
```
