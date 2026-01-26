# Chapter 01-06: Depth Testing & 3D Cube

## 학습 목표

Depth Buffer를 사용하여 3D 렌더링에서 올바른 가시성 판정을 하는 방법을 배웁니다.

## 주요 개념

- Depth Attachment - Render pass에 depth 추가
- Depth Format - D32_SFLOAT, D24_UNORM_S8_UINT 등
- Depth Testing - Fragment가 앞에 있는지 판정
- 3D Geometry - Cube (36 vertices, 6 faces)

## TODO

`main.cpp`에서 다음을 구현하세요:
- Depth image, memory, view 생성
- Render pass 수정 (depth attachment 추가)
- Pipeline에 depth test 활성화
- 3D 큐브 vertex 데이터 (36개)
- 회전하는 큐브 렌더링
