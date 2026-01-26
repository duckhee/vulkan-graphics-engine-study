# Chapter 01-08: Blinn-Phong Lighting

## 학습 목표

Blinn-Phong 조명 모델을 사용하여 사실적인 조명 효과를 구현하는 방법을 배웁니다.

## 주요 개념

- Vertex Normal - 표면의 방향 벡터
- Blinn-Phong Model - Ambient + Diffuse + Specular
- Per-Fragment Lighting - Fragment shader에서 조명 계산
- Normal Matrix - 모델 변환 시 normal 올바르게 변환

## TODO

`main.cpp`에서 다음을 구현하세요:
- Vertex에 normal 추가
- UBO에 light position/color 추가
- Fragment shader에 Blinn-Phong 계산
- ImGui로 light position 조절
