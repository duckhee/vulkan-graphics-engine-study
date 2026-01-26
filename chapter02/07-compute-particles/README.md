# Chapter 02-07: Compute Particles

## 개요
Vulkan **Compute Pipeline**을 사용하여 GPU에서 실시간 파티클 물리 시뮬레이션을 수행합니다.
**Storage Buffer (SSBO)**를 통해 Compute와 Graphics 파이프라인 간 데이터를 공유합니다.

## 학습 목표
- **Compute Pipeline** 생성 및 사용 (`VkComputePipelineCreateInfo`)
- **Storage Buffer (SSBO)** - Compute와 Graphics 간 데이터 공유
- **Compute Dispatch** (`vkCmdDispatch`) - GPU에서 병렬 연산
- **Compute-Graphics 동기화** - Semaphore를 통한 파이프라인 동기화
- **Point Sprite 렌더링** - `VK_PRIMITIVE_TOPOLOGY_POINT_LIST`
- **Additive Blending** - 파티클 효과를 위한 블렌딩

## 핵심 개념

### 1. Compute Pipeline
```cpp
VkComputePipelineCreateInfo pipelineInfo{};
pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
pipelineInfo.stage = compShaderStageInfo;  // Compute shader만 사용
pipelineInfo.layout = computePipelineLayout;

vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline);
```

### 2. Storage Buffer (SSBO)
```glsl
// Compute Shader에서
layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

// Vertex Shader에서 (readonly)
layout(std430, binding = 0) readonly buffer ParticleBuffer {
    Particle particles[];
};
```

### 3. Compute Dispatch
```cpp
uint32_t workGroupCount = (PARTICLE_COUNT + 255) / 256;  // local_size_x = 256
vkCmdDispatch(commandBuffer, workGroupCount, 1, 1);
```

### 4. Compute-Graphics 동기화
```cpp
// Compute 제출 시 Semaphore 신호
computeSubmitInfo.signalSemaphoreCount = 1;
computeSubmitInfo.pSignalSemaphores = &computeFinishedSemaphore;

// Graphics 제출 시 Semaphore 대기
VkSemaphore waitSemaphores[] = {computeFinishedSemaphore, imageAvailableSemaphore};
VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
```

## 셰이더 구조

### particle.comp (Compute Shader)
```glsl
layout(local_size_x = 256) in;

struct Particle {
    vec4 position;  // xyz = position, w = lifetime
    vec4 velocity;  // xyz = velocity, w = mass
    vec4 color;     // rgba
};

layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

void main() {
    uint index = gl_GlobalInvocationID.x;

    // 물리 시뮬레이션
    particles[index].velocity.y += params.gravity * params.deltaTime;
    particles[index].velocity.xyz *= params.damping;
    particles[index].position.xyz += particles[index].velocity.xyz * params.deltaTime;

    // 수명 감소
    particles[index].position.w -= params.deltaTime;

    // 리스폰
    if (particles[index].position.w <= 0.0 && params.respawnEnabled > 0.5) {
        // 새 위치와 속도로 재생성
    }
}
```

### particle.vert (Vertex Shader)
```glsl
layout(std430, binding = 0) readonly buffer ParticleBuffer {
    Particle particles[];
};

void main() {
    Particle p = particles[gl_VertexIndex];

    vec4 viewPos = render.view * vec4(p.position.xyz, 1.0);
    gl_Position = render.proj * viewPos;

    // 거리 기반 포인트 크기
    float dist = length(viewPos.xyz);
    gl_PointSize = render.pointSize * (1.0 / dist) * 20.0;
    gl_PointSize = clamp(gl_PointSize, 1.0, 64.0);
}
```

### particle.frag (Fragment Shader)
```glsl
void main() {
    // 포인트 스프라이트 원형 마스크
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord) * 2.0;
    float alpha = 1.0 - smoothstep(0.0, 1.0, dist);

    if (alpha < 0.01) discard;

    outColor = vec4(inColor.rgb, inColor.a * alpha);
}
```

## 파티클 시스템 특징

### 물리 시뮬레이션
- **중력 (Gravity)**: Y축 방향 가속도
- **감쇠 (Damping)**: 속도 감소 계수
- **어트랙터 (Attractor)**: 중심으로 끌어당기는 힘
- **바닥 충돌**: Y=0에서 반사

### 파티클 속성
- **Position**: 3D 위치 + 수명(lifetime)
- **Velocity**: 3D 속도 + 질량(mass)
- **Color**: RGBA 색상

### 렌더링
- **Point Sprite**: `gl_PointCoord`를 사용한 원형 파티클
- **Additive Blending**: 밝은 파티클 효과
- **거리 기반 크기**: 원근감 표현
- **수명 기반 효과**: 젊은 파티클의 발광 효과

## ImGui 컨트롤

| 컨트롤 | 기능 | 범위 |
|--------|------|------|
| Paused | 시뮬레이션 일시정지 | On/Off |
| Respawn | 파티클 재생성 | On/Off |
| Gravity | 중력 가속도 | -20 ~ 20 |
| Damping | 속도 감쇠 | 0.9 ~ 1.0 |
| Attractor | 어트랙터 강도 | 0 ~ 5 |
| Position | 발생 위치 | -5 ~ 5 |
| Range | 발생 범위 | 0.1 ~ 3 |
| Emit Speed | 발사 속도 | 0.5 ~ 10 |
| Point Size | 파티클 크기 | 1 ~ 50 |

## 빌드 및 실행

```bash
# 셰이더 컴파일 (이미 완료됨)
cd shaders
glslangValidator -V particle.comp -o particle_comp.spv
glslangValidator -V particle.vert -o particle_vert.spv
glslangValidator -V particle.frag -o particle_frag.spv

# 빌드
cd ..
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build .

# 실행
./ch02-07-compute-particles
```

## 파일 구조

```
07-compute-particles/
├── CMakeLists.txt
├── README.md
├── main.cpp              # 메인 애플리케이션 (~1400줄)
└── shaders/
    ├── particle.comp     # Compute shader - 물리 시뮬레이션
    ├── particle.vert     # Vertex shader - 위치 및 크기
    ├── particle.frag     # Fragment shader - 포인트 스프라이트
    ├── particle_comp.spv
    ├── particle_vert.spv
    └── particle_frag.spv
```

## 주요 Vulkan 구조체

### Compute Pipeline 생성
```cpp
VkComputePipelineCreateInfo pipelineInfo{};
pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
pipelineInfo.stage = compShaderStageInfo;
pipelineInfo.layout = computePipelineLayout;
```

### Storage Buffer 생성
```cpp
createBuffer(bufferSize,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    particleBuffer, particleBufferMemory);
```

### Descriptor Set Layout (Compute)
```cpp
bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;  // Particle buffer
bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;  // Sim params
```

## 기술적 세부사항

### 파티클 수
- 기본값: **8,192** 파티클
- Work Group Size: 256 (local_size_x)
- Dispatch Count: ceil(8192 / 256) = 32

### 동기화 전략
1. Compute Fence로 이전 프레임 compute 완료 대기
2. Compute Command Buffer 제출 (Semaphore 신호)
3. Graphics Fence로 이전 프레임 graphics 완료 대기
4. Graphics Command Buffer 제출 (Compute Semaphore 대기)

### 블렌딩 설정
```cpp
colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Additive
colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
```

## 예상 결과

- 1024x768 윈도우
- 어두운 파란색 배경에 수천 개의 발광 파티클
- 중력의 영향을 받아 떨어지는 파티클
- 바닥에서 튀어오르는 효과
- ImGui 패널에서 실시간 파라미터 조절 가능
