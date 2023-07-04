cmake_minimum_required(VERSION 3.23)

project(GAME VERSION 1.0)

# Enable C++20 features
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} 
   src/main.cpp
)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${SDL2_INCLUDE_DIR} 
)

file(GLOB SOURCES 
   "${PROJECT_SOURCE_DIR}/src/*.cpp"
) 

target_link_libraries(${PROJECT_NAME}
   ${SOURCES}
   ${SDL2_LIBRARY}  
)

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message("inc_dirs = ${inc_dirs}")
message(STATUS "Project source dir: " ${PROJECT_SOURCE_DIR})
