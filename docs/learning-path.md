# Vulkan 학습 경로 가이드

이 프로젝트는 다양한 학습 스타일에 맞춰 **3가지 트랙**을 제공합니다.

## 학습 트랙 비교

| 트랙 | 대상 | 코드량 | 특징 |
|------|------|--------|------|
| **Track A** (Abstracted) | 빠르게 결과를 보고 싶은 초보자 | ~50줄 | 추상화된 클래스 사용 |
| **Track B** (Modular) | 점진적으로 배우고 싶은 학습자 | 70→210줄 | 모듈 조합 방식 |
| **Track C** (Standalone) | 모든 것을 직접 구현하고 싶은 학습자 | ~800줄 | 완전 독립형 |

---

## Track A: Abstracted (추상화 버전)

**위치**: `chapter01/track-a-abstracted/`

### 특징
- `VulkanBase` 클래스가 모든 Vulkan 초기화를 처리
- 최소한의 코드로 삼각형 렌더링
- Vulkan의 "결과"를 먼저 보고 싶을 때 적합

### 예제 코드
```cpp
#include <vk_base.h>
#include <vk_window.h>

int main() {
    vk::Window window(800, 600, "Vulkan Triangle");
    vk::VulkanBase app;
    app.init(window);

    while (!window.shouldClose()) {
        window.pollEvents();
        app.drawFrame();
    }
    return 0;
}
```

### 장점
- 즉시 실행 가능한 결과
- Vulkan API 세부사항 숨김
- 빠른 프로토타이핑

### 단점
- 내부 동작 이해 어려움
- 커스터마이징 제한

---

## Track B: Modular (모듈 조합 버전)

**위치**: `chapter01/track-b-modular/`

### 특징
- 독립적인 Vulkan 모듈 조합
- 단계별로 기능 추가
- 각 모듈의 역할 명확히 이해 가능

### 단계별 구성

| 단계 | 폴더 | 코드량 | 학습 내용 |
|------|------|--------|----------|
| 1 | `01-instance-device/` | ~70줄 | Instance, Device 생성 |
| 2 | `02-add-swapchain/` | ~90줄 | Swapchain 추가 |
| 3 | `03-add-renderpass/` | ~100줄 | Render Pass, Framebuffers |
| 4 | `04-add-pipeline/` | ~210줄 | Pipeline, Commands, ImGui |

### 사용되는 모듈
```
common/modules/
├── vk_instance.h/cpp    # Vulkan Instance + Validation
├── vk_device.h/cpp      # Physical/Logical Device
├── vk_swapchain.h/cpp   # Swapchain + Image Views
├── vk_renderpass.h/cpp  # Render Pass + Framebuffers
├── vk_pipeline.h/cpp    # Graphics Pipeline
├── vk_commands.h/cpp    # Command Pool/Buffer + Sync
└── vk_imgui.h/cpp       # ImGui 통합
```

### 예제 코드 (Step 4)
```cpp
#include <vk_window.h>
#include <vk_instance.h>
#include <vk_device.h>
#include <vk_swapchain.h>
#include <vk_renderpass.h>
#include <vk_pipeline.h>
#include <vk_commands.h>
#include <vk_imgui.h>

int main() {
    vk::Window window(800, 600, "Track B - Triangle");

    // 각 모듈을 순서대로 초기화
    vk::VulkanInstance vulkanInstance;
    vulkanInstance.create("Triangle", true);

    vk::VulkanDevice vulkanDevice;
    vulkanDevice.pickPhysicalDevice(vulkanInstance.getInstance(), surface);
    vulkanDevice.createLogicalDevice(...);

    // ... 나머지 모듈 초기화

    while (!window.shouldClose()) {
        // 렌더링 루프
    }
}
```

### 장점
- Vulkan 구성요소별 이해
- 점진적 학습 곡선
- 모듈 재사용 가능

### 단점
- Track A보다 더 많은 코드
- 모듈 간 의존성 이해 필요

---

## Track C: Standalone (독립형 버전)

**위치**: `chapter02/` 예제들

### 특징
- 모든 Vulkan 코드를 한 파일에 포함
- 외부 의존성 없이 완전 독립
- Vulkan Tutorial 스타일

### Chapter 02 예제들

| 예제 | 셰이더 | 학습 내용 |
|------|--------|----------|
| `01-vertex-animation` | Vertex | Push Constants, 시간 기반 애니메이션 |
| `02-fragment-gradients` | Fragment | 그라데이션, 색상 믹싱 |
| `03-fragment-textures` | Fragment | 절차적 텍스처 |
| `04-fragment-lighting` | Fragment | UBO, Phong 라이팅 |
| `05-geometry-normals` | Geometry | 법선 벡터 시각화 |
| `06-geometry-explosion` | Geometry | 폭발 효과 |
| `07-compute-particles` | Compute | 파티클 시뮬레이션 |
| `08-compute-image-filter` | Compute | 이미지 필터 |
| `09-tessellation-terrain` | Tessellation | 지형 생성 |
| `10-raytracing-shadows` | Ray Tracing | 실시간 그림자 |

### ShaderExampleBase 사용
Chapter 02 예제들은 `ShaderExampleBase` 클래스를 상속받아 보일러플레이트 코드를 줄입니다:

```cpp
#include <shader_example_base.h>

class MyExample : public ch02::ShaderExampleBase {
public:
    MyExample() : ShaderExampleBase("My Example", 800, 600) {}

protected:
    void createGraphicsPipeline() override { /* 파이프라인 생성 */ }
    void recordCommandBuffer(VkCommandBuffer cmd, uint32_t idx) override { /* 드로우 콜 */ }
    void renderImGui() override { /* UI 구성 */ }
    void onUpdate() override { /* 매 프레임 업데이트 */ }
};
```

---

## 권장 학습 순서

### 완전 초보자
1. **Track A** → 결과 먼저 확인
2. **Track B** Step 1-4 → 각 구성요소 이해
3. **Track C** 01-03 → 셰이더 프로그래밍 시작

### Vulkan 경험자
1. **Track B** Step 4 → 모듈 구조 파악
2. **Track C** 04-06 → 고급 셰이더 기법
3. **Track C** 07-10 → Compute/Tessellation/Ray Tracing

### 빠른 프로토타이핑
1. **Track A** 또는 **Track B** Step 4 사용
2. 셰이더만 수정하여 다양한 효과 테스트

---

## 프로젝트 구조

```
claude-vulkan/
├── common/                      # 공통 라이브러리
│   ├── modules/                 # Track B용 모듈
│   ├── vk_base.h/cpp           # Track A용 베이스 클래스
│   └── vk_window.h/cpp         # 윈도우 관리
│
├── chapter01/                   # 기본 삼각형
│   ├── track-a-abstracted/     # 추상화 버전
│   └── track-b-modular/        # 모듈 조합 버전
│       ├── 01-instance-device/
│       ├── 02-add-swapchain/
│       ├── 03-add-renderpass/
│       └── 04-add-pipeline/
│
└── chapter02/                   # 셰이더 프로그래밍
    ├── common/                  # ShaderExampleBase
    └── 01~10/                   # 개별 예제
```
