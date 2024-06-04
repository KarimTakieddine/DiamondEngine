cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

set(LIB_DIR_PREFIX lib)
set(BIN_TARGET_ARCH x64)
set(PREBUILT_CONFIG Release)

set(GLEW_DIR ../../glew-2.1.0-win32/glew-2.1.0)
set(GLEW_LIBRARY_DIR "${GLEW_DIR}/${LIB_DIR_PREFIX}/${PREBUILT_CONFIG}/${BIN_TARGET_ARCH}")
set(GLM_DIR ../../glm)
set(STB_DIR ../../stb)
set(COMMON_DIR ../common)
set(CONFIG_DIR ../config)
set(DEBUG_DIR ../debug)
set(GAME_DIR ../game)
set(INCLUDE_DIR include)
set(SRC_DIR src)

set(SOURCES
    ${SRC_DIR}/allocator/GLAllocator.cpp
    ${SRC_DIR}/component/MaterialRenderComponent.cpp
    ${SRC_DIR}/component/TransformRenderComponent.cpp
    ${SRC_DIR}/mesh/Mesh.cpp
    ${SRC_DIR}/mesh/SharedMeshStore.cpp
    ${SRC_DIR}/graphics/Camera.cpp
    ${SRC_DIR}/graphics/Transform.cpp
    ${SRC_DIR}/graphics/Renderer.cpp
    ${SRC_DIR}/graphics/RenderingSubsystem.cpp
    ${SRC_DIR}/graphics/UniformBufferAgent.cpp
    ${SRC_DIR}/shader/Shader.cpp
    ${SRC_DIR}/shader/ShaderProgram.cpp
    ${SRC_DIR}/shader/SharedShaderStore.cpp
    ${SRC_DIR}/texture/TextureLoader.cpp)

add_library(libDiamondEngineGraphics STATIC ${SOURCES})

target_include_directories(libDiamondEngineGraphics PRIVATE
    "${GLEW_DIR}/${INCLUDE_DIR}"
    ${GLM_DIR}
    ${STB_DIR}
    "${COMMON_DIR}/${INCLUDE_DIR}"
    "${CONFIG_DIR}/${INCLUDE_DIR}/config"
    "${CONFIG_DIR}/${INCLUDE_DIR}/parser"
    "${DEBUG_DIR}/${INCLUDE_DIR}/debug"
    "${DEBUG_DIR}/${INCLUDE_DIR}/event"
    "${GAME_DIR}/${INCLUDE_DIR}/component"
    "${GAME_DIR}/${INCLUDE_DIR}/game"
    "${INCLUDE_DIR}/allocator"
    "${INCLUDE_DIR}/component"
    "${INCLUDE_DIR}/mesh"
    "${INCLUDE_DIR}/graphics"
    "${INCLUDE_DIR}/shader"
    "${INCLUDE_DIR}/texture"
)

find_library(GLEW
    NAMES glew32
    HINTS ${GLEW_LIBRARY_DIR}
)

target_link_libraries(libDiamondEngineGraphics PRIVATE opengl32 ${GLEW})
