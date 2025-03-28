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

# spine-runtimes

set(SPINE_RUNTIMES_DIR spine-runtimes/spine-cpp)

add_subdirectory(${SPINE_RUNTIMES_DIR})

set(SOURCES
    ${SRC_DIR}/animation/SpriteSheetLoader.cpp
    ${SRC_DIR}/component/SpriteAnimationPlayer.cpp
    ${SRC_DIR}/component/Collider2DComponent.cpp
    ${SRC_DIR}/component/ComponentFactory.cpp
    ${SRC_DIR}/game/GameInstance.cpp
    ${SRC_DIR}/game/GameInstanceManager.cpp
    ${SRC_DIR}/game/LevelLoader.cpp
    ${SRC_DIR}/physics/CollisionSolver2D.cpp)

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
    "${GRAPHICS_DIR}/include/mesh"
    "${GRAPHICS_DIR}/include/shader"
    "${GRAPHICS_DIR}/include/texture"
    "${INCLUDE_DIR}/animation"
    "${INCLUDE_DIR}/component"
    "${INCLUDE_DIR}/game"
    "${INCLUDE_DIR}/physics"
)

target_link_libraries(libDiamondEngineGame PRIVATE spine-cpp)
