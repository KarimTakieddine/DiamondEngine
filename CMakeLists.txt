cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(DiamondEngine)

set(CMAKE_CXX_STANDARD 17)

# Common Variables:

set(BIN_DIR_PREFIX bin)
set(LIB_DIR_PREFIX lib)
set(BIN_TARGET_ARCH x64)
set(PREBUILT_CONFIG Release)

set(GLEW_DIR glew-2.1.0-win32/glew-2.1.0)
set(GLFW_DIR glfw)
set(GLM_DIR glm)

set(INCLUDE_DIR include)
set(SRC_DIR src)
set(SHADERS_DIR shaders)
set(SCENES_DIR scenes)
set(TEXTURES_DIR textures)
set(CONFIG_DIR config)

set(INCLUDE_DIR ${INCLUDE_DIR} PARENT_SCOPE)
set(SRC_DIR ${SRC_DIR} PARENT_SCOPE)
set(SHADERS_DIR ${SHADERS_DIR} PARENT_SCOPE)
set(SCENES_DIR ${SCENES_DIR} PARENT_SCOPE)

# GLEW:

set(GLEW_BINARY_DIR     "${GLEW_DIR}/${BIN_DIR_PREFIX}/${PREBUILT_CONFIG}/${BIN_TARGET_ARCH}")
set(GLEW_LIBRARY_DIR    "${GLEW_DIR}/${LIB_DIR_PREFIX}/${PREBUILT_CONFIG}/${BIN_TARGET_ARCH}")

set(GLEW_BINARY_DIR ${GLEW_BINARY_DIR} PARENT_SCOPE)

# GLFW:

set(GLFW_LIBRARY_TYPE STATIC)
set(GLFW_BUILD_DOCS FALSE)
set(GLFW_BUILD_TESTS FALSE)

# pugixml

set(PUGI_XML_DIR pugixml)

# STB

set (STB_DIR stb)

add_subdirectory(${GLFW_DIR})
add_subdirectory(${PUGI_XML_DIR})

# DiamondEngine:

set(SOURCES
    ${SRC_DIR}/utility/DeltaTimer.cpp
    ${SRC_DIR}/utility/Logger.cpp
    ${SRC_DIR}/utility/LogManager.cpp
    ${SRC_DIR}/engine/Window.cpp
    ${SRC_DIR}/allocator/GLAllocator.cpp
    ${SRC_DIR}/engine/GraphicsContext.cpp
    ${SRC_DIR}/mesh/Mesh.cpp
    ${SRC_DIR}/component/MeshRenderer.cpp
    ${SRC_DIR}/mesh/SharedMeshStore.cpp
    ${SRC_DIR}/gameobject/GameObject.cpp
    ${SRC_DIR}/builder/GameObjectBuilder.cpp
    ${SRC_DIR}/shader/Shader.cpp
    ${SRC_DIR}/shader/ShaderProgram.cpp
    ${SRC_DIR}/parser/ShaderMetadataParser.cpp
    ${SRC_DIR}/shader/SharedShaderStore.cpp
    ${SRC_DIR}/texture/TextureLoader.cpp
    ${SRC_DIR}/parser/TextureMetadataParser.cpp
    ${SRC_DIR}/rendering/Material.cpp
    ${SRC_DIR}/rendering/Transform.cpp
    ${SRC_DIR}/rendering/Camera.cpp
    ${SRC_DIR}/rendering/RenderSequence.cpp
    ${SRC_DIR}/config/KeyboardConfig.cpp
    ${SRC_DIR}/config/GameObjectConfig.cpp
    ${SRC_DIR}/config/MaterialConfig.cpp
    ${SRC_DIR}/config/SceneConfig.cpp
    ${SRC_DIR}/config/MeshRendererConfig.cpp
    ${SRC_DIR}/config/WindowConfig.cpp
    ${SRC_DIR}/config/EngineConfig.cpp
    ${SRC_DIR}/input/Keyboard.cpp
    ${SRC_DIR}/input/Input.cpp
    ${SRC_DIR}/parser/KeyboardConfigParser.cpp
    ${SRC_DIR}/parser/MeshRendererConfigParser.cpp
    ${SRC_DIR}/parser/GameObjectConfigParser.cpp
    ${SRC_DIR}/parser/Vector2Parser.cpp
    ${SRC_DIR}/parser/Vector3Parser.cpp
    ${SRC_DIR}/parser/SizeParser.cpp
    ${SRC_DIR}/parser/EngineConfigParser.cpp
    ${SRC_DIR}/parser/WindowConfigParser.cpp
    ${SRC_DIR}/builder/MeshRendererBuilder.cpp
    ${SRC_DIR}/parser/MaterialConfigParser.cpp
    ${SRC_DIR}/parser/SceneConfigParser.cpp
    ${SRC_DIR}/scene/Scene.cpp
    ${SRC_DIR}/builder/SceneBuilder.cpp
    ${SRC_DIR}/scene/SceneLoader.cpp
    ${SRC_DIR}/behaviour/RotateBehaviour.cpp
    ${SRC_DIR}/parser/RotateBehaviourConfigParser.cpp
    ${SRC_DIR}/builder/RotateBehaviourBuilder.cpp
    ${SRC_DIR}/behaviour/BehaviourRegistry.cpp
    ${SRC_DIR}/utility/File.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${GLEW_DIR}/include"
    "${GLFW_DIR}/include"
    ${GLM_DIR}
    "${PUGI_XML_DIR}/src"
    ${STB_DIR}
    "${INCLUDE_DIR}/diamond_engine"
    "${INCLUDE_DIR}/diamond_engine/allocator"
    "${INCLUDE_DIR}/diamond_engine/behaviour"
    "${INCLUDE_DIR}/diamond_engine/builder"
    "${INCLUDE_DIR}/diamond_engine/common"
    "${INCLUDE_DIR}/diamond_engine/component"
    "${INCLUDE_DIR}/diamond_engine/config"
    "${INCLUDE_DIR}/diamond_engine/engine"
    "${INCLUDE_DIR}/diamond_engine/gameobject"
    "${INCLUDE_DIR}/diamond_engine/input"
    "${INCLUDE_DIR}/diamond_engine/mesh"
    "${INCLUDE_DIR}/diamond_engine/parser"
    "${INCLUDE_DIR}/diamond_engine/rendering"
    "${INCLUDE_DIR}/diamond_engine/scene"
    "${INCLUDE_DIR}/diamond_engine/shader"
    "${INCLUDE_DIR}/diamond_engine/texture"
    "${INCLUDE_DIR}/diamond_engine/utility")

find_library(GLEW
    NAMES glew32
    HINTS ${GLEW_LIBRARY_DIR}
)

add_dependencies(${PROJECT_NAME} ${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 ${GLEW} ${GLFW_DIR} pugixml)
