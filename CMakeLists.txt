cmake_minimum_required(VERSION 3.23)

project(GAME VERSION 1.0)

# Enable C++20 features
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
   "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} 
   src/main.cpp
)

target_include_directories(${PROJECT_NAME}
   PRIVATE ${PROJECT_SOURCE_DIR}/include
   PUBLIC ${PROJECT_SOURCE_DIR}/src/Game
   PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_sources(${PROJECT_NAME}
   PRIVATE ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
   ${SDL2_LIBRARY}
)
