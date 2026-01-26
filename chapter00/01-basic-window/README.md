# Chapter 00-01: Basic Window

## 학습 목표

ImGui의 가장 기본적인 사용법을 배웁니다.

## 주요 개념

- `ImGui::Begin()` / `ImGui::End()` - 윈도우 생성
- `ImGui::Text()` - 텍스트 표시

## 실습 내용

`main.cpp`의 `renderImGui()` 함수에서 TODO 부분을 채워주세요.

## 예제

```cpp
ImGui::Begin("My First Window");
ImGui::Text("Hello, ImGui!");
ImGui::Text("This is a basic window.");
ImGui::End();
```
