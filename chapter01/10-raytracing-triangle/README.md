# Chapter 01-10: Ray Tracing Triangle

## 학습 목표

Acceleration Structure를 생성하고 ray-triangle intersection을 구현하는 방법을 배웁니다.

## 주요 개념

- BLAS (Bottom-Level AS) - Geometry 데이터
- TLAS (Top-Level AS) - Scene 구성
- Geometry Description - Vertex/Index buffer를 AS에 전달
- Closest Hit Shader (.rchit) - Ray가 geometry에 맞았을 때
- Buffer Device Address - AS에서 buffer 참조

## TODO

`main.cpp`에서 다음을 구현하세요:
- Vertex/Index buffer 생성 (삼각형 1개)
- BLAS 생성
- TLAS 생성
- Closest hit shader 작성 (barycentric 색상)
- Ray tracing으로 삼각형 렌더링
