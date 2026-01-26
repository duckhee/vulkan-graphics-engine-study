# Chapter 01-11: Ray Tracing Reflections

## 학습 목표

재귀 ray tracing과 custom intersection shader를 사용하여 반사 효과를 구현하는 방법을 배웁니다.

## 주요 개념

- Recursive Ray Tracing - Hit shader에서 새 ray 생성
- Custom Intersection Shader (.rint) - 구 교차 테스트
- AABB (Axis-Aligned Bounding Box) - Custom geometry 경계
- Multiple BLAS - 바닥 + 구
- Ray Payload - Ray 간 데이터 전달

## TODO

`main.cpp`에서 다음을 구현하세요:
- 바닥 BLAS 생성 (평면)
- 구 BLAS 생성 (AABB + intersection shader)
- TLAS에 여러 instance 추가
- Intersection shader 작성 (구 교차 계산)
- Closest hit shader에서 반사 ray 생성
- 재귀 ray tracing (1 bounce)
