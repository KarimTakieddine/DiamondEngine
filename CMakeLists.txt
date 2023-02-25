cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(DiamondEngine)

set(CMAKE_CXX_STANDARD 17)

set(GLEW_DIR glew-2.1.0-win32/glew-2.1.0)
set(GLFW_DIR glfw)
set(GLM_DIR glm)
set(INCLUDE_DIR include)
set(SRC_DIR src)

set(GLEW_BINARY_DIR "${GLEW_DIR}/bin/Release/x64")
set(GLEW_LIBRARY_DIR "${GLEW_DIR}/lib/Release/x64")

set(GLFW_LIBRARY_TYPE STATIC)
set(GLFW_BUILD_DOCS FALSE)
set(GLFW_BUILD_TESTS FALSE)

add_subdirectory(${GLFW_DIR})

set(SOURCES
    ${SRC_DIR}/DeltaTimer.cpp
    ${SRC_DIR}/Logger.cpp
    ${SRC_DIR}/LogManager.cpp
    ${SRC_DIR}/Window.cpp
    ${SRC_DIR}/GLAllocator.cpp
    ${SRC_DIR}/GraphicsContext.cpp
    ${SRC_DIR}/Mesh.cpp
    ${SRC_DIR}/MeshRenderer.cpp
    ${SRC_DIR}/DiamondEngine.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${GLEW_DIR}/include" "${GLFW_DIR}/include" ${GLM_DIR} ${INCLUDE_DIR})

add_dependencies(${PROJECT_NAME} ${GLFW_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARY_DIR} "${GLFW_BINARY_DIR}/src")
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glew32 glfw3)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${GLEW_BINARY_DIR}/glew32.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
