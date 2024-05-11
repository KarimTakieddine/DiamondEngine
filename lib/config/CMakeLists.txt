cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_DIR ../../glew-2.1.0-win32/glew-2.1.0)
set(COMMON_DIR ../common)
set(INPUT_DIR ../input)
set(INCLUDE_DIR include)
set(SRC_DIR src)

# glm

set(GLM_DIR ../../glm)

# pugixml

set(PUGI_XML_DIR ../../pugixml)

set(SOURCES
    ${SRC_DIR}/config/ControllerConfig.cpp
    ${SRC_DIR}/config/EngineConfig.cpp
    ${SRC_DIR}/config/KeyboardConfig.cpp
    ${SRC_DIR}/config/GameInstanceConfig.cpp
    ${SRC_DIR}/config/GameSceneConfig.cpp
    ${SRC_DIR}/config/MaterialComponentConfig.cpp
    ${SRC_DIR}/config/TransformComponentConfig.cpp
    ${SRC_DIR}/config/WindowConfig.cpp
    ${SRC_DIR}/parser/ComponentConfigParser.cpp
    ${SRC_DIR}/parser/ControllerConfigParser.cpp
    ${SRC_DIR}/parser/EngineConfigParser.cpp
    ${SRC_DIR}/parser/GameInstanceConfigParser.cpp
    ${SRC_DIR}/parser/GameSceneConfigParser.cpp
    ${SRC_DIR}/parser/KeyboardConfigParser.cpp
    ${SRC_DIR}/parser/ShaderMetadataParser.cpp
    ${SRC_DIR}/parser/TextureMetadataParser.cpp
    ${SRC_DIR}/parser/Vector2Parser.cpp
    ${SRC_DIR}/parser/Vector3Parser.cpp
    ${SRC_DIR}/parser/Vector4Parser.cpp
    ${SRC_DIR}/parser/WindowConfigParser.cpp)

add_library(libDiamondEngineConfig STATIC ${SOURCES})

target_include_directories(libDiamondEngineConfig PRIVATE
    "${GLEW_DIR}/include"
    ${GLM_DIR}
    "${PUGI_XML_DIR}/${SRC_DIR}"
    "${COMMON_DIR}/${INCLUDE_DIR}"
    "${INPUT_DIR}/include/input"
    "${INCLUDE_DIR}/config"
    "${INCLUDE_DIR}/parser"
)

target_link_libraries(libDiamondEngineConfig PRIVATE pugixml)
