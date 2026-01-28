# Vulkan Ray Tracing Learning Project

Vulkan과 Ray Tracing을 단계별로 학습하기 위한 프로젝트입니다.

## 학습 트랙

이 프로젝트는 **3가지 학습 트랙**을 제공합니다:

| 트랙 | 대상 | 특징 |
|------|------|------|
| **Track A** | 빠르게 결과를 보고 싶은 초보자 | 추상화된 클래스로 ~50줄 코드 |
| **Track B** | 점진적으로 배우고 싶은 학습자 | 모듈 조합으로 단계별 학습 |
| **Track C** | 모든 것을 직접 구현하고 싶은 학습자 | 완전 독립형 코드 |

자세한 내용은 [docs/learning-path.md](docs/learning-path.md)를 참고하세요.

---

## 요구사항

### 필수 소프트웨어
- **CMake** 3.20 이상
- **C++ 컴파일러** (C++20 지원)
  - Windows: Visual Studio 2019 이상
  - macOS: Xcode Command Line Tools
  - Linux: GCC 10+ 또는 Clang 12+
- **Vulkan SDK** 1.3 이상
  - 다운로드: https://vulkan.lunarg.com/

### 외부 라이브러리 (vcpkg 자동 관리)
- GLFW (윈도우/입력)
- ImGui (GUI, Vulkan + GLFW 백엔드)
- GLM (수학 라이브러리)

---

## 빌드 방법

### 1. vcpkg 설치 (최초 1회)

**macOS/Linux**:
```bash
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg && ./bootstrap-vcpkg.sh

# 환경변수 설정 (권장)
echo 'export VCPKG_ROOT=~/vcpkg' >> ~/.zshrc
echo 'export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake' >> ~/.zshrc
source ~/.zshrc
```

**Windows**:
```cmd
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg && .\bootstrap-vcpkg.bat
```

### 2. 프로젝트 빌드

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE
cmake --build build
```

**참고**: 첫 빌드 시 vcpkg가 의존성을 다운로드하고 컴파일하므로 5-10분 소요될 수 있습니다.

### 3. 실행

```bash
# Chapter 01 - Track B Step 4 (완전한 삼각형)
./build/chapter01/track-b-modular/04-add-pipeline/ch01-trackb-04

# Chapter 02 - Vertex Animation
./build/chapter02/01-vertex-animation/ch02-01-vertex-animation
```

---

## 프로젝트 구조

```
claude-vulkan/
├── common/                          # 공통 라이브러리
│   ├── modules/                     # Track B용 독립 모듈
│   │   ├── vk_instance.h/cpp       # Vulkan Instance
│   │   ├── vk_device.h/cpp         # Physical/Logical Device
│   │   ├── vk_swapchain.h/cpp      # Swapchain
│   │   ├── vk_renderpass.h/cpp     # Render Pass
│   │   ├── vk_pipeline.h/cpp       # Graphics Pipeline
│   │   ├── vk_commands.h/cpp       # Command Buffers
│   │   └── vk_imgui.h/cpp          # ImGui 통합
│   ├── vk_base.h/cpp               # Track A용 베이스 클래스
│   └── vk_window.h/cpp             # 윈도우 관리
│
├── chapter01/                       # 기본 삼각형 렌더링
│   ├── track-a-abstracted/         # 추상화 버전
│   │   └── 01-triangle-imgui/
│   └── track-b-modular/            # 모듈 조합 버전
│       ├── 01-instance-device/     # Step 1: Instance + Device
│       ├── 02-add-swapchain/       # Step 2: + Swapchain
│       ├── 03-add-renderpass/      # Step 3: + Render Pass
│       └── 04-add-pipeline/        # Step 4: + Pipeline + ImGui
│
├── chapter02/                       # 셰이더 프로그래밍
│   ├── common/                      # ShaderExampleBase
│   ├── 01-vertex-animation/        # Push Constants 애니메이션
│   ├── 02-fragment-gradients/      # 그라데이션 효과
│   ├── 03-fragment-textures/       # 절차적 텍스처
│   ├── 04-fragment-lighting/       # UBO + Phong 라이팅
│   ├── 05-geometry-normals/        # 법선 시각화
│   ├── 06-geometry-explosion/      # 폭발 효과
│   ├── 07-compute-particles/       # 파티클 시뮬레이션
│   ├── 08-compute-image-filter/    # 이미지 필터
│   ├── 09-tessellation-terrain/    # 지형 생성
│   └── 10-raytracing-shadows/      # 실시간 그림자
│
└── docs/                            # 문서
    └── learning-path.md            # 학습 경로 가이드
```

---

## 학습 단계

### Chapter 01: Triangle + ImGui
**목표**: Vulkan 기본 설정 및 삼각형 렌더링

- Track A: 추상화된 클래스로 빠르게 결과 확인
- Track B: 모듈별로 Vulkan 구성요소 이해
  - Step 1: Instance, Physical/Logical Device
  - Step 2: Swapchain, Image Views
  - Step 3: Render Pass, Framebuffers
  - Step 4: Pipeline, Commands, ImGui

### Chapter 02: Shader Programming
**목표**: 다양한 셰이더 기법 학습

| 예제 | 셰이더 타입 | 핵심 개념 |
|------|------------|----------|
| 01 | Vertex | Push Constants, 시간 기반 애니메이션 |
| 02 | Fragment | 그라데이션, 색상 믹싱 |
| 03 | Fragment | 절차적 텍스처 생성 |
| 04 | Fragment | Uniform Buffer Objects, Phong 라이팅 |
| 05-06 | Geometry | 법선 시각화, 폭발 효과 |
| 07-08 | Compute | 파티클, 이미지 필터 |
| 09 | Tessellation | 동적 지형 생성 |
| 10 | Ray Tracing | 실시간 그림자 |

---

## 문제 해결

### Vulkan SDK를 찾을 수 없음
- Vulkan SDK가 설치되어 있는지 확인
- 환경변수 `VULKAN_SDK`가 설정되어 있는지 확인

### vcpkg 오류
- `builtin-baseline` 오류 시 vcpkg.json 확인
- vcpkg 최신 버전으로 업데이트: `cd ~/vcpkg && git pull && ./bootstrap-vcpkg.sh`

### Validation Layer 경고
- Release 빌드 시에는 자동 비활성화됩니다
- `-DCMAKE_BUILD_TYPE=Release` 옵션 사용

---

## 참고 자료
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan Spec](https://www.khronos.org/registry/vulkan/)
- [NVIDIA Vulkan Ray Tracing Tutorial](https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR)
