cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_DIR        ../../glew-2.1.0-win32/glew-2.1.0)
set(GLM_DIR         ../../glm)
set(COMMON_DIR      ../common)
set(GRAPHICS_DIR    ../graphics)
set(INCLUDE_DIR     include)
set(SRC_DIR         src)

set(SOURCES
    ${SRC_DIR}/font/Font.cpp)

add_library(libDiamondEngineFont STATIC ${SOURCES})

target_include_directories(libDiamondEngineFont PRIVATE
    "${GLEW_DIR}/${INCLUDE_DIR}"
    ${GLM_DIR}
    "${COMMON_DIR}/${INCLUDE_DIR}"
    "${GRAPHICS_DIR}/${INCLUDE_DIR}/component"
    "${GRAPHICS_DIR}/${INCLUDE_DIR}/graphics"
    "${INCLUDE_DIR}/font"
)
