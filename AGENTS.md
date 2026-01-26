# Repository Guidelines

## Project Structure & Module Organization
- `main.cpp`는 현재 진입점이자 주요 소스 파일입니다.
- `CMakeLists.txt`는 빌드 타깃(`claude_vulkan`)과 C++20 설정을 정의합니다.
- `cmake-build-debug/`는 로컬 빌드 산출물 디렉터리입니다(생성물로 취급, 수동 편집 금지).

## Build, Test, and Development Commands
- 구성 및 빌드:
  - `cmake -S . -B build`
  - `cmake --build build`
- 실행(빌드 디렉터리 기준):
  - `./build/claude_vulkan`
- 현재 테스트는 없습니다. 테스트가 추가되면 CMake 기반 실행은 `ctest --test-dir build`를 우선 사용하세요.

## Coding Style & Naming Conventions
- 언어 표준: `CMakeLists.txt`에 지정된 C++20.
- 들여쓰기: 4칸, 중괄호는 새 줄(Allman 스타일), `main.cpp`와 일치.
- 네이밍: 파일은 `snake_case`, 변수는 `lowerCamelCase` 또는 `snake_case`를 사용하고 짧고 명확하게 작성합니다.
- 포맷터/린터는 미구성입니다. 도입 시 이 문서에 기록하고 일관성을 유지하세요.

## Testing Guidelines
- 테스트 프레임워크는 아직 없습니다. 추가 시 `tests/` 디렉터리를 만들고 `test_<feature>.cpp` 형식을 권장합니다.
- 새 테스트는 CMake에 연결하고 `ctest`로 실행 가능해야 합니다.

## Commit & Pull Request Guidelines
- 이 저장소에는 규칙을 유추할 Git 히스토리가 없습니다.
- 커밋 메시지는 간결한 동사 원형으로 작성합니다(예: `Add Vulkan instance setup`).
- PR에는 변경 요약, 빌드/실행 메모, 동작 변화 시 관련 출력 또는 스크린샷을 포함하세요.

## Agent-Specific Notes
- 기존 CMake 구조를 유지하면서 최소한의 추가를 지향합니다.
- 새 디렉터리, 도구, 워크플로가 생기면 이 문서를 업데이트하세요.
