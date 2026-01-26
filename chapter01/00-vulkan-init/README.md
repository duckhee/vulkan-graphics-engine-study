# Chapter 01-00: Vulkan Initialization

## 학습 목표

Vulkan의 기본 초기화 과정을 이해합니다.

## 주요 개념

이 예제는 Vulkan의 핵심 초기화 단계를 보여줍니다:

### 1. Instance 생성
- Vulkan API와의 연결 설정
- Validation Layer 활성화 (디버그 빌드)
- macOS의 경우 MoltenVK 확장 활성화

### 2. Physical Device 선택
- 사용 가능한 GPU 탐색
- 적절한 Queue Family 찾기 (Graphics + Present 지원)

### 3. Logical Device 생성
- Physical Device를 기반으로 논리 장치 생성
- Graphics Queue와 Present Queue 획득

### 4. Swapchain 설정
- 화면에 렌더링할 이미지 체인 생성
- Surface Format, Present Mode 선택
- Image Views 생성

### 5. Render Pass 생성
- 렌더링 파이프라인의 구조 정의
- Color Attachment 설정

### 6. Graphics Pipeline 생성
- Shader 모듈 로드 (Vertex + Fragment)
- Pipeline Layout 설정
- Viewport, Rasterizer 등 설정

### 7. Framebuffers 생성
- Swapchain 이미지마다 Framebuffer 생성

### 8. Command Buffers
- 렌더링 명령 기록
- 각 프레임마다 실행

### 9. Synchronization
- Semaphores와 Fences 사용
- GPU-CPU 동기화

## 실행 결과

이 예제를 실행하면:
- 800x600 크기의 검은 윈도우가 표시됩니다
- ImGui UI는 없습니다 (순수 Vulkan만)
- 콘솔에 초기화 과정이 출력됩니다

## 코드 구조

```cpp
VulkanInitApp : public VulkanBase
  - renderImGui() override: 비어있음 (ImGui 없음)

main():
  - Window 생성
  - VulkanInitApp 초기화
  - 메인 루프 (drawFrame() 호출)
```

## 빌드 및 실행

```bash
cd cmake-build-debug
cmake --build . --target chapter0100VulkanInit

cd chapter01/00-vulkan-init
./ch01-00
```

## 학습 포인트

1. **vk_common 라이브러리 활용**
   - `VulkanBase` 클래스가 모든 초기화를 처리
   - `init()` 함수 하나로 전체 설정 완료

2. **최소 Vulkan 앱**
   - ImGui 없이 순수 Vulkan만 사용
   - 화면 클리어만 수행

3. **다음 단계**
   - 01-triangle-imgui: 삼각형 렌더링 + ImGui
   - 셰이더를 통한 기하학 렌더링

## 참고

- Vulkan 초기화는 많은 단계가 필요하지만, `vk_common` 라이브러리가 대부분을 처리합니다
- 실제 프로젝트에서는 이러한 초기화 코드를 재사용 가능한 라이브러리로 만드는 것이 일반적입니다
- 이 예제는 기본 구조를 이해하기 위한 것으로, 세부 구현은 `common/vk_base.cpp`를 참고하세요
