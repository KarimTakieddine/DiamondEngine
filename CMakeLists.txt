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

set(GLEW_BINARY_DIR     "${GLEW_DIR}/${BIN_DIR_PREFIX}/${PREBUILT_CONFIG}/${BIN_TARGET_ARCH}")
set(GLEW_LIBRARY_DIR    "${GLEW_DIR}/${LIB_DIR_PREFIX}/${PREBUILT_CONFIG}/${BIN_TARGET_ARCH}")

# GLFW:

set(GLFW_LIBRARY_TYPE STATIC)
set(GLFW_BUILD_DOCS FALSE)
set(GLFW_BUILD_TESTS FALSE)

# pugixml

set(PUGI_XML_DIR pugixml)

add_subdirectory(${GLFW_DIR})
add_subdirectory(${PUGI_XML_DIR})

# DiamondEngine:

set(SOURCES
    ${SRC_DIR}/DeltaTimer.cpp
    ${SRC_DIR}/Logger.cpp
    ${SRC_DIR}/LogManager.cpp
    ${SRC_DIR}/Window.cpp
    ${SRC_DIR}/GLAllocator.cpp
    ${SRC_DIR}/GraphicsContext.cpp
    ${SRC_DIR}/Mesh.cpp
    ${SRC_DIR}/MeshRenderer.cpp
    ${SRC_DIR}/SharedMeshStore.cpp
    ${SRC_DIR}/GameObject.cpp
    ${SRC_DIR}/GameObjectBuilder.cpp
    ${SRC_DIR}/GameObjectDesigner.cpp
    ${SRC_DIR}/Shader.cpp
    ${SRC_DIR}/ShaderProgram.cpp
    ${SRC_DIR}/ShaderMetadataParser.cpp
    ${SRC_DIR}/SharedShaderStore.cpp
    ${SRC_DIR}/Material.cpp
    ${SRC_DIR}/Transform.cpp
    ${SRC_DIR}/Camera.cpp
    ${SRC_DIR}/Scene.cpp
    ${SRC_DIR}/SpriteBuilder.cpp
    ${SRC_DIR}/File.cpp
    ${SRC_DIR}/DiamondEngine.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${GLEW_DIR}/include" "${GLFW_DIR}/include" ${GLM_DIR} "${PUGI_XML_DIR}/src" ${INCLUDE_DIR})

add_dependencies(${PROJECT_NAME} ${GLFW_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARY_DIR} "${GLFW_BINARY_DIR}/src")
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glew32 glfw3 pugixml)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${GLEW_BINARY_DIR}/glew32.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SHADERS_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADERS_DIR}
)
