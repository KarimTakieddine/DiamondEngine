cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_DIR        ../../glew-2.1.0-win32/glew-2.1.0)
set(GLM_DIR         ../../glm)
set(COMMON_DIR      ../common)
set(FONT_DIR        ../font)
set(GRAPHICS_DIR    ../graphics)
set(INCLUDE_DIR     include)
set(SRC_DIR         src)

set(SOURCES
	${SRC_DIR}/debug/Debugger.cpp
    ${SRC_DIR}/event/GLAllocationEvent.cpp
    ${SRC_DIR}/event/GLMemoryRequest.cpp
    ${SRC_DIR}/event/SpriteAnimationPlay.cpp
    ${SRC_DIR}/handler/GLAllocationEventHandler.cpp
    ${SRC_DIR}/handler/GLMemoryRequestHandler.cpp
    ${SRC_DIR}/handler/SpriteAnimationPlayHandler.cpp)

add_library(libDiamondEngineDebug STATIC ${SOURCES})

target_include_directories(libDiamondEngineDebug PRIVATE
    "${GLEW_DIR}/${INCLUDE_DIR}"
    ${GLM_DIR}
    "${COMMON_DIR}/${INCLUDE_DIR}"
    "${GRAPHICS_DIR}/${INCLUDE_DIR}/texture"
    "${FONT_DIR}/${INCLUDE_DIR}/font"
    ${INCLUDE_DIR}/debug
    ${INCLUDE_DIR}/event
    ${INCLUDE_DIR}/handler
)
