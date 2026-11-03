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
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3       CONFIG REQUIRED)
find_package(SDL3_ttf   CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)
find_package(OpenAL     CONFIG REQUIRED)

# 1. 自动扫描 src 目录下的所有 .cpp 文件
file(GLOB_RECURSE MANGO_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# 2. 直接把扫描出来的列表丢给可执行文件
add_executable(MangoEngine ${MANGO_SOURCES})

if (MSVC)
    target_compile_options(MangoEngine PRIVATE /utf-8)
endif()

target_include_directories(MangoEngine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MangoEngine 
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    OpenAL::OpenAL
)


if (WIN32)
    add_custom_command(TARGET MangoEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:MangoEngine>
            $<TARGET_FILE_DIR:MangoEngine>
        COMMAND_EXPAND_LISTS
        COMMENT "Copying runtime DLLs to output directory...")
endif()