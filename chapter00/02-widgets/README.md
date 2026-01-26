# Chapter 00-02: Widgets

## 학습 목표

ImGui의 기본 위젯들을 배웁니다.

## 주요 개념

- `ImGui::Button()` - 버튼
- `ImGui::Checkbox()` - 체크박스
- `ImGui::RadioButton()` - 라디오 버튼
- `ImGui::SliderFloat()` - 슬라이더

## 실습 내용

다양한 위젯들을 추가하고 상태를 저장해보세요.

## 예제

```cpp
static bool checked = false;
static float value = 0.5f;

if (ImGui::Button("Click Me")) {
    // Button clicked
}
ImGui::Checkbox("Enable", &checked);
ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);
```
