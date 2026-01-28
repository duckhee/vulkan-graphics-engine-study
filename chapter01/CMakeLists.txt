# Chapter 01: Vulkan + Ray Tracing Examples
#
# 학습 트랙 설명:
# - Track A (기존): vk_common 라이브러리 사용, 추상화된 버전
# - Track B (신규): 모듈을 하나씩 추가하며 점진적 학습

# ==============================
# Track A: Abstracted (기존 예제)
# ==============================
add_subdirectory(00-vulkan-init)
add_subdirectory(01-triangle-imgui)

# 아래는 TODO 상태 (셰이더 파일 없음)
# add_subdirectory(02-vertex-buffers)
# add_subdirectory(03-index-buffers)
# add_subdirectory(04-uniform-buffers)
# add_subdirectory(05-texture-mapping)
# add_subdirectory(06-depth-testing)
# add_subdirectory(07-model-loading)
# add_subdirectory(08-lighting)
# add_subdirectory(09-raytracing-setup)
# add_subdirectory(10-raytracing-triangle)
# add_subdirectory(11-raytracing-reflection)

# ==============================
# Track B: Modular (신규)
# 모듈을 하나씩 추가하며 점진적으로 학습
# ==============================
add_subdirectory(track-b-modular)
