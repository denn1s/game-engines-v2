cmake_minimum_required(VERSION 4.0)

project(GAME VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)
find_package(imgui REQUIRED)
find_package(Lua REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/external/rlImGui/rlImGui.cpp"
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/src
    PUBLIC ${PROJECT_SOURCE_DIR}/external/rlImGui
    PUBLIC ${PROJECT_SOURCE_DIR}/external/FastNoise/Cpp
    PUBLIC ${LUA_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    raylib
    imgui
    ${LUA_LIBRARIES}
)
