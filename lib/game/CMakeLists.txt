cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_DIR ../../glew-2.1.0-win32/glew-2.1.0)
set(COMMON_DIR ../common)
set(CONFIG_DIR ../config)
set(GRAPHICS_DIR ../graphics)
set(INCLUDE_DIR include)
set(SRC_DIR src)

# glm

set(GLM_DIR ../../glm)

set(SOURCES
    ${SRC_DIR}/game/GameInstance.cpp
    ${SRC_DIR}/game/GameInstanceBuilder.cpp
    ${SRC_DIR}/game/GameInstanceManager.cpp)

add_library(libDiamondEngineGame STATIC ${SOURCES})

target_include_directories(libDiamondEngineGame PRIVATE
    "${GLEW_DIR}/include"
    ${GLM_DIR}
    "${COMMON_DIR}/${INCLUDE_DIR}"
    "${CONFIG_DIR}/include/config"
    "${CONFIG_DIR}/include/parser"
    "${GRAPHICS_DIR}/include/allocator"
    "${GRAPHICS_DIR}/include/component"
    "${GRAPHICS_DIR}/include/graphics"
    "${GRAPHICS_DIR}/include/shader"
    "${INCLUDE_DIR}/allocator"
    "${INCLUDE_DIR}/component"
    "${INCLUDE_DIR}/game"
)
