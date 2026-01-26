# Chapter 01-07: OBJ Model Loading

## 학습 목표

외부 3D 모델 파일(OBJ)을 로딩하여 렌더링하는 방법을 배웁니다.

## 주요 개념

- OBJ 파일 포맷 - Wavefront OBJ
- tinyobjloader - OBJ 파싱 라이브러리
- 대용량 vertex/index 데이터 처리

## Dependencies

`vcpkg.json`에 추가 필요:
```json
"tinyobjloader"
```

## TODO

`main.cpp`에서 다음을 구현하세요:
- tinyobjloader로 .obj 파일 로딩
- Vertex/Index vector 생성
- 모델 렌더링
- ImGui로 모델 파일 선택 (선택적)
