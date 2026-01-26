# 08: Compute Image Filter

## 개요
Vulkan Compute Shader를 사용하여 실시간 이미지 필터링을 구현합니다.

## 학습 목표
- **Storage Image** (`image2D`) 사용법 이해
- **Compute Shader로 이미지 처리** 패턴 학습
- **Convolution Kernel** (3x3 필터) 구현
- **Color Transformation** 기법 이해
- **Compute-Graphics Pipeline 연동** 학습
- **Image Layout Transitions** 관리

## 주요 개념

### Storage Image
```glsl
layout(binding = 0, rgba8) uniform readonly image2D inputImage;
layout(binding = 1, rgba8) uniform writeonly image2D outputImage;
```
- `imageLoad()` / `imageStore()`로 픽셀 단위 접근
- `GENERAL` 또는 특수 레이아웃 필요

### 2D Compute Dispatch
```cpp
// 16x16 워크그룹으로 이미지 처리
uint32_t groupX = (width + 15) / 16;
uint32_t groupY = (height + 15) / 16;
vkCmdDispatch(cmd, groupX, groupY, 1);
```

### 구현된 필터들

| 필터 | 설명 | 커널/방식 |
|------|------|-----------|
| None | 원본 이미지 | pass-through |
| Blur | 흐림 효과 | 3x3 Box blur |
| Sharpen | 선명하게 | Edge enhancement |
| Edge | 윤곽선 검출 | Sobel operator |
| Emboss | 양각 효과 | Directional light |
| Grayscale | 흑백 변환 | Luminance weights |
| Invert | 색상 반전 | 1.0 - color |
| Sepia | 세피아 톤 | Color matrix |

### Convolution Kernels 예시

**Blur (Box Filter)**:
```
1/9  1/9  1/9
1/9  1/9  1/9
1/9  1/9  1/9
```

**Sharpen**:
```
 0   -1    0
-1    5   -1
 0   -1    0
```

**Edge Detection (Sobel)**:
```
Gx:             Gy:
-1   0   1      -1  -2  -1
-2   0   2       0   0   0
-1   0   1       1   2   1
magnitude = sqrt(Gx² + Gy²)
```

## 파일 구조
```
08-compute-image-filter/
├── CMakeLists.txt       # 빌드 설정
├── README.md            # 이 파일
├── main.cpp             # 메인 구현 (~1400줄)
└── shaders/
    ├── filter.comp      # 이미지 필터 compute shader
    ├── filter_comp.spv  # 컴파일된 compute shader
    ├── fullscreen.vert  # 전체화면 vertex shader
    ├── fullscreen_vert.spv
    ├── fullscreen.frag  # 텍스처 샘플링 fragment shader
    └── fullscreen_frag.spv
```

## 빌드 방법

### Standalone 빌드
```bash
cd chapter02/08-compute-image-filter
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build .
./bin/ch02-08
```

### 셰이더 컴파일
```bash
cd shaders
glslangValidator -V filter.comp -o filter_comp.spv
glslangValidator -V fullscreen.vert -o fullscreen_vert.spv
glslangValidator -V fullscreen.frag -o fullscreen_frag.spv
```

## ImGui 컨트롤

| 컨트롤 | 설명 |
|--------|------|
| Filter Type | 8가지 필터 선택 (콤보박스) |
| Intensity | 필터 강도 (0.0 ~ 2.0) |
| Vignette | 비네트 효과 ON/OFF |
| Image Info | 이미지 크기, 필터 정보 표시 |

## 핵심 구현

### 1. Storage Image 생성
```cpp
VkImageCreateInfo imageInfo{};
imageInfo.usage = VK_IMAGE_USAGE_STORAGE_BIT |
                  VK_IMAGE_USAGE_SAMPLED_BIT |
                  VK_IMAGE_USAGE_TRANSFER_DST_BIT;
imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
```

### 2. Image Layout Transition
```cpp
// UNDEFINED → GENERAL (초기화)
// GENERAL ↔ SHADER_READ_ONLY_OPTIMAL (compute/graphics 전환)
VkImageMemoryBarrier barrier{};
barrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
```

### 3. Compute Descriptor 설정
```cpp
// Input Image (Storage Image - readonly)
VkDescriptorImageInfo inputInfo{};
inputInfo.imageView = inputImageView;
inputInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

// Output Image (Storage Image - writeonly)
VkDescriptorImageInfo outputInfo{};
outputInfo.imageView = outputImageView;
outputInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
```

### 4. Fullscreen Triangle 기법
```glsl
// 버텍스 버퍼 없이 3개의 정점으로 전체 화면 커버
vec2 uv = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
gl_Position = vec4(uv * 2.0 - 1.0, 0.0, 1.0);
```

## 테스트 이미지

프로그램은 512x512 절차적 테스트 이미지를 생성합니다:
- **체커보드 패턴** - 선명도 테스트
- **원형 그라데이션** - 블러/에지 테스트
- **색상 그라데이션** - 색상 필터 테스트

## 파이프라인 흐름

```
[테스트 이미지 생성]
        ↓
[Input Image (Storage)]
        ↓
[Compute Pass: 필터 적용]
        ↓
[Output Image (Storage → Sampled)]
        ↓
[Graphics Pass: Fullscreen 렌더링]
        ↓
[Swapchain Image]
```

## 확장 아이디어

1. **추가 필터**: Gaussian blur, Bilateral filter, Bloom
2. **실시간 웹캠**: 외부 이미지 로딩
3. **다중 패스**: 여러 필터 체이닝
4. **커스텀 커널**: 사용자 정의 convolution
5. **히스토그램**: Compute로 히스토그램 계산

## 관련 리소스

- [Vulkan Storage Images](https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkDescriptorType.html)
- [Image Processing Kernels](https://en.wikipedia.org/wiki/Kernel_(image_processing))
- [GLSL imageLoad/imageStore](https://www.khronos.org/opengl/wiki/Image_Load_Store)
- [Fullscreen Triangle](https://www.saschawillems.de/blog/2016/08/13/vulkan-tutorial-on-rendering-a-fullscreen-quad-without-buffers/)
