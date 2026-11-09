cmake_minimum_required(VERSION 3.25)

project(rot VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(MINIAUDIO_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MINIAUDIO_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MINIAUDIO_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(MINIAUDIO_INSTALL OFF CACHE BOOL "" FORCE)
set(MINIAUDIO_NO_EXTRA_NODES ON CACHE BOOL "" FORCE)

function(rot_require_dependency name)
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/dep/${name}")
        message(FATAL_ERROR "Missing dependency: dep/${name}")
    endif()
endfunction()

foreach(dependency IN ITEMS glfw glad miniaudio stb nanovg log.c)
    rot_require_dependency("${dependency}")
endforeach()

add_subdirectory(dep/glfw EXCLUDE_FROM_ALL)

add_library(glad STATIC "${PROJECT_SOURCE_DIR}/dep/glad/generated/src/gl.c")
add_library(glad::glad ALIAS glad)
target_include_directories(glad PUBLIC "${PROJECT_SOURCE_DIR}/dep/glad/generated/include")
target_compile_features(glad PUBLIC c_std_99)

add_subdirectory(dep/miniaudio EXCLUDE_FROM_ALL)
add_library(miniaudio::miniaudio ALIAS miniaudio)

add_library(stb INTERFACE)
add_library(stb::stb ALIAS stb)
target_include_directories(stb INTERFACE "${PROJECT_SOURCE_DIR}/dep/stb")

add_library(vecmath INTERFACE)
add_library(vecmath::vecmath ALIAS vecmath)
target_include_directories(vecmath INTERFACE "${PROJECT_SOURCE_DIR}/dep/vecmath")


add_library(nanovg STATIC "${PROJECT_SOURCE_DIR}/dep/nanovg/src/nanovg.c")
add_library(nanovg::nanovg ALIAS nanovg)
target_include_directories(nanovg PUBLIC "${PROJECT_SOURCE_DIR}/dep/nanovg/src")
target_compile_features(nanovg PUBLIC c_std_99)
if(UNIX AND NOT APPLE)
    target_link_libraries(nanovg PUBLIC m)
endif()

add_library(log_c STATIC "${PROJECT_SOURCE_DIR}/dep/log.c/src/log.c")
add_library(log::log ALIAS log_c)
set_target_properties(log_c PROPERTIES OUTPUT_NAME log)
target_include_directories(log_c PUBLIC "${PROJECT_SOURCE_DIR}/dep/log.c/src")
target_compile_features(log_c PUBLIC c_std_99)

file(GLOB_RECURSE ROT_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/include/*.c"
)

file(GLOB_RECURSE ROT_HEADERS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.h"
    # "${PROJECT_SOURCE_DIR}/include/*.h"
)

add_executable(rot ${ROT_SOURCES} ${ROT_HEADERS})
target_compile_features(rot PRIVATE c_std_99)
target_compile_definitions(rot PRIVATE
    GLFW_INCLUDE_NONE
    "ROT_DEBUG=$<IF:$<CONFIG:Debug>,1,0>"
    "ROT_RELEASE=$<IF:$<CONFIG:Release>,1,0>"
)
# target_include_directories(rot PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(rot PRIVATE
    glfw
    glad::glad
    miniaudio::miniaudio
    vecmath::vecmath
    stb::stb
    nanovg::nanovg
    log::log
)

if(MSVC)
    target_compile_options(rot PRIVATE /W4)
else()
    target_compile_options(rot PRIVATE -Wall -Wextra -Wpedantic)
endif()
