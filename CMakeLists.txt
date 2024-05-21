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

# GLEW:

set(GLEW_BINARY_DIR "${GLEW_DIR}/${BIN_DIR_PREFIX}/${PREBUILT_CONFIG}/${BIN_TARGET_ARCH}")
set(GLEW_BINARY_DIR ${GLEW_BINARY_DIR} PARENT_SCOPE)

# GLFW:

set(GLFW_LIBRARY_TYPE STATIC)
set(GLFW_BUILD_DOCS FALSE)
set(GLFW_BUILD_TESTS FALSE)

add_subdirectory(${GLFW_DIR})

set(COMMON_DIR      "${LIB_DIR_PREFIX}/common")
set(CONFIG_DIR      "${LIB_DIR_PREFIX}/config")
set(AUDIO_DIR       "${LIB_DIR_PREFIX}/audio")
set(GRAPHICS_DIR    "${LIB_DIR_PREFIX}/graphics")
set(FONT_DIR        "${LIB_DIR_PREFIX}/font")
set(GAME_DIR        "${LIB_DIR_PREFIX}/game")
set(INPUT_DIR       "${LIB_DIR_PREFIX}/input")

add_subdirectory(${CONFIG_DIR})
add_subdirectory(${AUDIO_DIR})
add_subdirectory(${GRAPHICS_DIR})
add_subdirectory(${FONT_DIR})
add_subdirectory(${GAME_DIR})
add_subdirectory(${INPUT_DIR})

# pugixml

add_subdirectory(${PUGI_XML_DIR})

# irrKlang

set(IRR_KLANG_BINARY_DIR "${IRR_KLANG_DIR}/bin/winx64-visualStudio")
set(IRR_KLANG_BINARY_DIR ${IRR_KLANG_BINARY_DIR} PARENT_SCOPE)

# DiamondEngine:

set(SOURCES
    ${SRC_DIR}/engine/GraphicsContext.cpp
    ${SRC_DIR}/engine/Window.cpp
    ${SRC_DIR}/engine/GameEngine.cpp
    ${SRC_DIR}/utility/DeltaTimer.cpp
    ${SRC_DIR}/utility/File.cpp
    ${SRC_DIR}/utility/Logger.cpp
    ${SRC_DIR}/utility/LogManager.cpp)

add_library(DiamondEngine STATIC ${SOURCES})

target_include_directories(DiamondEngine PUBLIC
    "${GLEW_DIR}/include"
    "${GLFW_DIR}/include"
    ${GLM_DIR}
    ${STB_DIR}
    "${PUGI_XML_DIR}/${SRC_DIR}"
    "${LIB_DIR_PREFIX}/common/${INCLUDE_DIR}"
    "${CONFIG_DIR}/include/config"
    "${CONFIG_DIR}/include/parser"
    "${FONT_DIR}/include/component"
    "${FONT_DIR}/include/font"
    "${GAME_DIR}/include/animation"
    "${GAME_DIR}/include/component"
    "${GAME_DIR}/include/game"
    "${GAME_DIR}/include/physics"
    "${AUDIO_DIR}/include/audio"
    "${GRAPHICS_DIR}/include/allocator"
    "${GRAPHICS_DIR}/include/component"
    "${GRAPHICS_DIR}/include/mesh"
    "${GRAPHICS_DIR}/include/graphics"
    "${GRAPHICS_DIR}/include/shader"
    "${GRAPHICS_DIR}/include/texture"
    "${INPUT_DIR}/include/input"
    "${INCLUDE_DIR}/diamond_engine"
    "${INCLUDE_DIR}/diamond_engine/engine"
    "${INCLUDE_DIR}/diamond_engine/utility")

add_dependencies(DiamondEngine ${GLFW_DIR})
target_link_libraries(DiamondEngine PRIVATE
    libDiamondEngineAudio
    libDiamondEngineGraphics
    libDiamondEngineConfig
    libDiamondEngineFont
    libDiamondEngineGame
    libDiamondEngineInput)
