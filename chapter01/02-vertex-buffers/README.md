# Chapter 01-02: Vertex Buffers

## 학습 목표

Vertex Buffer를 사용하여 GPU 메모리로 vertex 데이터를 전송하는 방법을 배웁니다.

## 주요 개념

- `VkBuffer` - Vertex 데이터 저장
- Staging Buffer - CPU에서 접근 가능한 임시 버퍼
- Device Local Buffer - GPU 전용 고속 메모리
- Memory Types - DEVICE_LOCAL vs HOST_VISIBLE
- `vkCmdCopyBuffer()` - 버퍼 간 데이터 복사
- Vertex Input Binding - 파이프라인에 vertex 구조 전달

## TODO

`main.cpp`에서 다음을 구현하세요:
- Vertex 구조체 정의 (position, color)
- Staging buffer 생성
- Device local buffer 생성
- 데이터 복사
- Pipeline에 vertex input state 설정
