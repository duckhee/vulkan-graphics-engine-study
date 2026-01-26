# Chapter 01: Triangle with ImGui

## 학습 목표

이 챕터에서는 다음을 학습합니다:

1. **Vulkan 기본 설정**
   - Instance 생성
   - Physical Device 선택
   - Logical Device 생성
   - Swapchain 설정

2. **기본 렌더링 파이프라인**
   - Render Pass
   - Graphics Pipeline
   - Command Buffer

3. **삼각형 렌더링**
   - Vertex Shader
   - Fragment Shader
   - 하드코딩된 정점 데이터

4. **ImGui 통합**
   - ImGui 초기화
   - UI 렌더링
   - 실시간 파라미터 조정

## 단계별 구현

### Step 1: 프로젝트 구조 ✅
- CMake 설정
- 기본 파일 구조

### Step 2: Vulkan 기본 설정 (진행 예정)
- GLFW 윈도우 생성
- Vulkan Instance/Device
- Swapchain

### Step 3: 삼각형 렌더링 (진행 예정)
- Shader 작성
- Pipeline 생성
- 렌더링 루프

### Step 4: ImGui 통합 (진행 예정)
- ImGui 초기화
- 인터랙티브 UI
- 색상/회전 조정

## 빌드 및 실행

### 독립 빌드 (이 챕터만)
```bash
cd chapter01-triangle-imgui
cmake -S . -B build
cmake --build build
./build/chapter01
```

### 루트에서 빌드
```bash
# 프로젝트 루트에서
cmake -S . -B build
cmake --build build
./build/chapter01
```

## 실행 결과

### Step 1 (현재)
- 콘솔에 "Build system is working!" 출력
- Vulkan SDK 연결 확인

### Step 2 (예정)
- 800x600 검은 윈도우 표시

### Step 3 (예정)
- 컬러풀한 삼각형 렌더링

### Step 4 (예정)
- ImGui 패널과 삼각형 동시 렌더링
- 슬라이더로 색상/회전 조정

## 주요 파일

- `main.cpp` - 메인 애플리케이션
- `shaders/triangle.vert` - Vertex Shader (Step 3에서 추가)
- `shaders/triangle.frag` - Fragment Shader (Step 3에서 추가)

## 참고 사항

- Vulkan Validation Layer가 활성화되어 있어 디버그 메시지가 출력됩니다
- ImGui는 Vulkan 백엔드를 사용합니다
- 셰이더는 SPIR-V로 컴파일되어야 합니다 (glslangValidator 사용)