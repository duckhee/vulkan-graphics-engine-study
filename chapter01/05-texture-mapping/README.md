# Chapter 01-05: Texture Mapping

## 학습 목표

Texture를 생성하고 shader에서 sampling하는 방법을 배웁니다.

## 주요 개념

- `VkImage` - 2D 텍스처 이미지
- Image Layout Transition - 이미지 용도 변경
- `VkSampler` - 텍스처 필터링 설정
- Combined Image Sampler - Descriptor set에 바인딩

## TODO

`main.cpp`에서 다음을 구현하세요:
- 체커보드 텍스처 생성 (512x512)
- Image layout transition
- Sampler 생성
- Descriptor set에 sampler 바인딩
- Vertex에 texCoord 추가
