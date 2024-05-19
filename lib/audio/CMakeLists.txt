cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

set(COMMON_DIR ../common)
set(CONFIG_DIR ../config)
set(INCLUDE_DIR include)
set(SRC_DIR src)

# iirKlang

set(IRR_KLANG_DIR irrKlang-64bit-1.6.0)
set(IRR_KLANG_DIR ${IRR_KLANG_DIR} PARENT_SCOPE)
set(IRR_KLANG_DIR ../../${IRR_KLANG_DIR})

set(SOURCES
    ${SRC_DIR}/audio/AudioDevice.cpp
    ${SRC_DIR}/audio/AudioEngine.cpp
    ${SRC_DIR}/audio/AudioLoader.cpp)

add_library(libDiamondEngineAudio STATIC ${SOURCES})

target_include_directories(libDiamondEngineAudio PRIVATE
    "${IRR_KLANG_DIR}/${INCLUDE_DIR}"
    "${COMMON_DIR}/${INCLUDE_DIR}"
    "${CONFIG_DIR}/${INCLUDE_DIR}/config"
    "${CONFIG_DIR}/${INCLUDE_DIR}/parser"
    "${INCLUDE_DIR}/audio"
)

set(IRR_KLANG_LIBRARY_DIR "${IRR_KLANG_DIR}/lib/Winx64-visualStudio")
find_library(IRR_KLANG
    NAMES irrKlang
    HINTS ${IRR_KLANG_LIBRARY_DIR}
)

target_link_libraries(libDiamondEngineAudio PRIVATE ${IRR_KLANG})
