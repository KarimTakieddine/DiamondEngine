cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_DIR ../../glew-2.1.0-win32/glew-2.1.0)
set(GLFW_DIR ../../glfw)
set(COMMON_DIR ../common)
set(INCLUDE_DIR include)
set(SRC_DIR src)

# glm

set(GLM_DIR ../../glm)

set(SOURCES
    ${SRC_DIR}/input/Input.cpp
    ${SRC_DIR}/input/Keyboard.cpp)

if (WIN32)
    list(APPEND SOURCES ${SRC_DIR}/input/XInputController.cpp)
endif()

add_library(libDiamondEngineInput STATIC ${SOURCES})

target_include_directories(libDiamondEngineInput PRIVATE
    "${GLEW_DIR}/include"
    "${GLFW_DIR}/include"
    ${GLM_DIR}
    "${COMMON_DIR}/${INCLUDE_DIR}"
    "${INCLUDE_DIR}/input"
)

target_link_libraries(libDiamondEngineInput PRIVATE glfw Xinput)
