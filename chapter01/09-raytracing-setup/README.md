# Chapter 01-09: Ray Tracing Pipeline Setup

## 학습 목표

Vulkan Ray Tracing extension을 활성화하고 ray tracing pipeline을 생성하는 방법을 배웁니다.

## 주요 개념

- Ray Tracing Extensions - VK_KHR_ray_tracing_pipeline
- Ray Generation Shader (.rgen) - 각 픽셀에서 ray 생성
- Miss Shader (.rmiss) - Ray가 아무것도 안 맞았을 때
- Shader Binding Table (SBT) - Shader 핸들 테이블
- Storage Image - Ray tracing 출력 이미지

## TODO

`main.cpp`에서 다음을 구현하세요:
- RT extension 지원 확인
- Ray generation shader 작성
- Miss shader 작성 (하늘 그라데이션)
- RT pipeline 생성
- SBT 생성
- Storage image 생성
- vkCmdTraceRaysKHR 호출
