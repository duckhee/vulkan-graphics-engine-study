# Chapter 01-04: Uniform Buffers & MVP Transform

## 학습 목표

Uniform Buffer와 Descriptor Set을 사용하여 shader에 동적 데이터를 전달하는 방법을 배웁니다.

## 주요 개념

- Uniform Buffer Object (UBO) - 모든 vertex가 공유하는 데이터
- Descriptor Set - Shader에 리소스 바인딩
- MVP 변환 - Model, View, Projection 행렬
- Per-frame 업데이트 - 매 프레임마다 UBO 갱신

## TODO

`main.cpp`에서 다음을 구현하세요:
- UBO 구조체 (model, view, proj 행렬)
- Descriptor set layout 생성
- Uniform buffer 생성 (per frame)
- Descriptor sets 생성
- 매 프레임 model 행렬 회전
