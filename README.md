# Vulkan Ray Tracing Learning Project

Vulkan과 Ray Tracing을 단계별로 학습하기 위한 프로젝트입니다.

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

이 라이브러리들은 **vcpkg**를 통해 자동으로 설치됩니다.

## 빌드 방법

### 1. vcpkg 설치 (최초 1회)

**macOS/Linux**:
```bash
# vcpkg 클론
git clone https://github.com/Microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg
./bootstrap-vcpkg.sh

# 환경변수 설정 (선택, 하지만 권장)
echo 'export VCPKG_ROOT=~/vcpkg' >> ~/.zshrc  # 또는 ~/.bashrc
echo 'export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake' >> ~/.zshrc
source ~/.zshrc
```

**Windows**:
```cmd
# vcpkg 클론
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat

# 환경변수 설정 (시스템 환경변수에 추가)
VCPKG_ROOT=C:\vcpkg
CMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

### 2. 프로젝트 빌드

**환경변수를 설정한 경우**:
```bash
# 전체 프로젝트 빌드
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE
cmake --build build

# 특정 챕터만 빌드
cd chapter01-triangle-imgui
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE
cmake --build build
```

**환경변수를 설정하지 않은 경우**:
```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

**주의**: 첫 빌드 시 vcpkg가 의존성을 다운로드하고 컴파일하므로 5-10분 소요될 수 있습니다.

### 3. 실행
```bash
# 전체 빌드에서 실행
./build/chapter01-triangle-imgui/chapter01

# 챕터 폴더에서 빌드한 경우
cd chapter01-triangle-imgui
./build/chapter01
```

## 학습 단계

### Chapter 01: Triangle + ImGui
- Vulkan 기본 설정 (Instance, Device, Swapchain)
- 삼각형 렌더링
- ImGui 통합 및 인터랙티브 UI

### 향후 챕터 (예정)
- Chapter 02: Vertex Buffers & Textures
- Chapter 03: 3D Models & Lighting
- Chapter 04: Ray Tracing Basics
- ...

## 프로젝트 구조

```
claude-vulkan/
├── common/              # 공통 Vulkan 유틸리티 라이브러리
├── chapter01-*/         # 각 학습 챕터
├── external/            # 외부 라이브러리 (Git Submodules)
└── shaders/             # 공유 셰이더 파일
```

## 문제 해결

### Vulkan SDK를 찾을 수 없음
- Vulkan SDK가 설치되어 있는지 확인
- 환경변수 `VULKAN_SDK`가 설정되어 있는지 확인

### Validation Layer 경고
- Vulkan SDK의 Validation Layer가 올바르게 설치되었는지 확인
- Release 빌드 시에는 Validation Layer가 비활성화됩니다

## 참고 자료
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan Spec](https://www.khronos.org/registry/vulkan/)
- [NVIDIA Vulkan Ray Tracing Tutorial](https://github.com/nvpro-samples/vk_raytracing_tutorial_KHR)