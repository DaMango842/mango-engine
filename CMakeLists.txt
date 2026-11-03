# CMakeLists.txt

cmake_minimum_required(VERSION 3.26)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif ()

project(MangoEngine LANGUAGES C CXX)

# if (MSVC)
#     add_compile_options(/utf-8)
# endif()

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

find_package(sdl3 CONFIG REQUIRED)
find_package(sdl3_ttf REQUIRED)
find_package(OpenAL CONFIG REQUIRED)

add_executable(MangoEngine 
    src/main.cpp 
    "src/Resource/Font.cpp"
    "include/Utils/MangoPtr_SDL.h"
    "include/Audio/MangoAudio.h" 
    "include/Audio/AudioBuffer.h"
    "include/Loader/AudioLoader.h" 
    "include/Loader/IAudioLoader.h"
    src/Renderer/SDLRenderer.cpp
)

if (MSVC)
    target_compile_options(MangoEngine PRIVATE /utf-8)
endif()

target_include_directories(MangoEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MangoEngine 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    OpenAL::OpenAL
)


add_custom_command(TARGET MangoEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:SDL3::SDL3>/SDL3.dll"
        $<TARGET_FILE_DIR:MangoEngine>
    COMMENT "Copying SDL3.dll to output directory..."
)

# OpenAL

add_custom_command(TARGET MangoEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:OpenAL::OpenAL>/OpenAL32.dll"
        $<TARGET_FILE_DIR:MangoEngine>
    COMMENT "Copying OpenAL32.dll to output directory..."
)