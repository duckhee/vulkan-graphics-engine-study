# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**claude-vulkan**은 Vulkan과 Ray Tracing을 단계별로 학습하기 위한 프로젝트입니다.
- 각 챕터가 독립적으로 빌드 및 실행 가능
- ImGui를 활용한 인터랙티브 학습
- vcpkg를 통한 의존성 관리

## Build System

### 빌드 도구
- **CMake** 3.20 이상 필요
- **C++20** 표준 사용
- **vcpkg** 패키지 매니저 사용

### vcpkg 의존성 관리

이 프로젝트는 **vcpkg Manifest 모드**를 사용합니다.

**의존성 목록** (`vcpkg.json`):
- `glfw3` - 윈도우 및 입력 관리
- `imgui[glfw-binding]` - GUI 라이브러리 (Vulkan 백엔드는 수동 추가)
- `glm` - 수학 라이브러리 (벡터/행렬)

**주의**: ImGui Vulkan 백엔드(`imgui_impl_vulkan.h/cpp`)는 vcpkg에서 빌드 문제가 있어 `common/` 폴더에 수동으로 포함되어 있습니다. Vulkan SDK는 시스템에 별도 설치 필요합니다.

**vcpkg 설치** (최초 1회):
```bash
# vcpkg 클론 및 부트스트랩
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh  # macOS/Linux
# 또는
~/vcpkg/bootstrap-vcpkg.bat  # Windows
```

**환경변수 설정** (선택, 하지만 강력 추천):
```bash
# ~/.zshrc 또는 ~/.bashrc에 추가
export VCPKG_ROOT=~/vcpkg
export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### 빌드 명령어

**전체 프로젝트 빌드**:
```bash
# vcpkg toolchain 사용
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build

# 실행
./build/chapter01
```

**특정 챕터만 빌드**:
```bash
cd chapter01-triangle-imgui
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/chapter01
```

**CLion 사용 시**:
- `Settings → Build → CMake → CMake options`에 추가:
  ```
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
  ```

## Project Structure

```
claude-vulkan/
├── CMakeLists.txt              # 루트 빌드 설정
├── vcpkg.json                  # 의존성 정의
├── CLAUDE.md                   # 이 파일
├── AGENTS.md                   # 추가 개발 가이드
├── README.md                   # 프로젝트 소개
├── common/                     # 공통 Vulkan 유틸리티
│   ├── vk_base.h/cpp          # Vulkan 초기화 베이스 클래스
│   ├── vk_window.h/cpp        # 윈도우 관리
│   ├── imgui_impl_vulkan.h/cpp # ImGui Vulkan 백엔드
│   └── CMakeLists.txt
├── chapter01-triangle-imgui/   # Chapter 01
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── shaders/
│   │   ├── triangle.vert      # Vertex shader
│   │   ├── triangle.frag      # Fragment shader
│   │   ├── vert.spv           # Compiled SPIR-V
│   │   └── frag.spv           # Compiled SPIR-V
│   └── README.md
└── build/                      # vcpkg가 관리 (gitignore)
```

## Common Library Architecture

**vk_common** 라이브러리는 모든 챕터에서 재사용되는 Vulkan 보일러플레이트를 제공합니다.

### 주요 클래스

**VulkanBase** (`vk_base.h/cpp`): 41KB - 모든 Vulkan 초기화 및 렌더링 로직
- Vulkan Instance 생성 (Validation Layer 포함, macOS MoltenVK 지원)
- Physical Device 선택 (GPU)
- Logical Device 및 Queue 생성 (Graphics + Present)
- Swapchain 및 Image Views 관리
- Render Pass 생성 (Color Attachment)
- Graphics Pipeline 생성 (Shader 로딩, Pipeline Layout)
- Framebuffers 생성
- Command Pool & Command Buffers 관리
- Synchronization (Semaphores, Fences, Double Buffering)
- ImGui 초기화 및 렌더링 통합
- `drawFrame()` - 메인 렌더링 루프
- `renderImGui()` - ImGui UI 빌드
- Shader 로딩 헬퍼 (`readFile()`, `createShaderModule()`)

**Window** (`vk_window.h/cpp`): 1.2KB
- GLFW 윈도우 생성 및 관리
- 이벤트 폴링 (`pollEvents()`)
- Vulkan Surface 호환 (GLFW_NO_API)
- 윈도우 닫기 감지 (`shouldClose()`)

**ImGui Vulkan Backend** (`imgui_impl_vulkan.h/cpp`): 87KB
- ImGui Vulkan 렌더링 백엔드
- vcpkg의 imgui[glfw-binding]과 함께 사용
- Descriptor Pool 관리

**사용 패턴** (chapter01):
```cpp
int main() {
    vk::Window window(800, 600, "Vulkan Triangle");
    vk::VulkanBase vulkanApp;
    vulkanApp.init(window);

    while (!window.shouldClose()) {
        window.pollEvents();
        vulkanApp.drawFrame();  // 삼각형 + ImGui 렌더링
    }

    return 0;
}
```

## Chapter 01 Details

### 구현된 기능
- **Vulkan 렌더링**: Instance, Device, Swapchain, Pipeline 완전 구현
- **삼각형 렌더링**: RGB 그라데이션 (Vertex Shader에 하드코딩)
- **ImGui UI**: FPS 표시, 인터랙티브 패널
- **총 코드**: ~3,300 줄 (common 라이브러리 포함)

### 실행 방법
```bash
./build/chapter01
```

### 예상 결과
- 800x600 윈도우
- 중앙에 RGB 그라데이션 삼각형
- 왼쪽 상단에 "Vulkan Triangle" ImGui 패널 (FPS 표시)
- ESC 또는 창 닫기로 종료

### Shader 수정 방법
```bash
cd chapter01-triangle-imgui/shaders
# .vert 또는 .frag 파일 수정
glslangValidator -V triangle.vert -o vert.spv
glslangValidator -V triangle.frag -o frag.spv
# 프로그램 재실행 (재빌드 불필요)
```

## Development Workflow

### 새 챕터 추가하기

1. **폴더 생성**:
   ```bash
   mkdir chapterXX-feature-name
   cd chapterXX-feature-name
   ```

2. **CMakeLists.txt 작성**:
   ```cmake
   add_executable(chapterXX main.cpp)
   target_link_libraries(chapterXX PRIVATE vk_common)
   ```

3. **루트 CMakeLists.txt에 추가**:
   ```cmake
   add_subdirectory(chapterXX-feature-name)
   ```

4. **README.md 작성** (학습 목표, 구현 내용)

### Shader 컴파일

셰이더는 SPIR-V로 컴파일되어야 합니다.

**현재**: 각 챕터의 shaders 폴더에서 수동 컴파일
```bash
cd chapter01-triangle-imgui/shaders
glslangValidator -V triangle.vert -o vert.spv
glslangValidator -V triangle.frag -o frag.spv
```

**향후**: CMake 자동 컴파일 추가 예정

**중요**: 셰이더를 수정한 후에는 반드시 재컴파일 필요!

## Vulkan Validation Layers

Debug 빌드에서는 Vulkan Validation Layer가 자동으로 활성화됩니다.

**에러 메시지 확인**:
- 터미널에 Vulkan Validation Layer 메시지 출력
- 심각도: ERROR, WARNING, INFO

**비활성화** (Release 빌드):
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release ...
```

## Code Style

### C++ 스타일
- **들여쓰기**: 4 spaces
- **중괄호**: Allman style (새 줄에 여는 중괄호)
- **네이밍**:
  - 클래스: `PascalCase` (예: `VulkanBase`)
  - 함수/변수: `camelCase` (예: `initVulkan`)
  - 상수: `UPPER_SNAKE_CASE` (예: `MAX_FRAMES_IN_FLIGHT`)

### 파일 구조
- 헤더 파일: `.h`
- 구현 파일: `.cpp`
- 셰이더: `.vert`, `.frag`, `.comp`, `.rgen`, `.rchit`, `.rmiss`

## Requirements

### 필수 설치
- **Vulkan SDK** 1.3 이상 (https://vulkan.lunarg.com/)
- **vcpkg** (위의 설치 방법 참고)
- **CMake** 3.20+
- **C++20 지원 컴파일러**

### 하드웨어
- **GPU**: Vulkan 1.3 지원
- **Ray Tracing** (Chapter 13+): RTX 20xx 이상 또는 AMD RDNA 2 이상

## Troubleshooting

### "Vulkan SDK not found"
- Vulkan SDK 설치 확인
- 환경변수 `VULKAN_SDK` 설정 확인

### "vcpkg dependencies failed"
- vcpkg가 올바르게 설치되었는지 확인
- `CMAKE_TOOLCHAIN_FILE` 경로가 정확한지 확인

### ImGui Vulkan 백엔드
- `imgui[glfw-binding]`만 vcpkg로 설치
- `imgui_impl_vulkan.h/cpp`는 `common/` 폴더에 수동 포함
- macOS vcpkg의 vulkan-loader 빌드 이슈 회피

## Chapter Learning Path

각 챕터는 점진적으로 난이도가 증가하도록 설계되었습니다.

**현재 진행 상황**:
- [x] **Chapter 01: Triangle + ImGui** ✅ 완료
  - [x] Step 1: 프로젝트 구조 + vcpkg 설정
  - [x] Step 2: Vulkan 초기화 (Instance, Device, Swapchain)
  - [x] Step 3: 삼각형 렌더링 (Shaders + Pipeline)
  - [x] Step 4: ImGui 통합 (FPS 표시)

**실행 결과**: 800x600 윈도우에 RGB 그라데이션 삼각형 + ImGui UI 패널

**향후 계획**:
- Chapter 02: Vertex Buffers & Index Buffers
- Chapter 03: Textures & Samplers
- Chapter 04: 3D Models & Depth Testing
- Chapter 05: Lighting (Phong/PBR)
- Chapter 06+: Ray Tracing

## References

- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan Guide](https://github.com/KhronosGroup/Vulkan-Guide)
- [ImGui](https://github.com/ocornut/imgui)
- [vcpkg](https://vcpkg.io/)
