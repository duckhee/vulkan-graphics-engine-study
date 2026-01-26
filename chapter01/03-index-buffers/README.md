# Chapter 01-03: Index Buffers

## 학습 목표

Index Buffer를 사용하여 vertex 재사용과 메모리 효율을 높이는 방법을 배웁니다.

## 주요 개념

- `VkBuffer` (Index용) - Index 데이터 저장
- `vkCmdBindIndexBuffer()` - Index buffer 바인딩
- `vkCmdDrawIndexed()` - Indexed draw call
- Vertex 재사용 - 4개 vertex로 6개 삼각형 정점 표현

## TODO

`main.cpp`에서 다음을 구현하세요:
- Index buffer 생성
- 4개 vertex + 6개 index로 정사각형 렌더링
- vkCmdDrawIndexed 사용
