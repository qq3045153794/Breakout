cmake_minimum_required(VERSION 3.10)
project(window)


message("SOURCE: ${PROJECT_SOURCE_DIR}")

set(CMAKE_BUILD_TYPE,"Debug")
set(SRC 
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/../../src/render/ParticleGenerator.cpp
    ${PROJECT_SOURCE_DIR}/../../src/tools/ResourceManager.cpp    
    ${PROJECT_SOURCE_DIR}/../../src/tools/Shader.cpp
    ${PROJECT_SOURCE_DIR}/../../src/tools/Texture2D.cpp
    ${PROJECT_SOURCE_DIR}/../../src/block/BallObject.cpp
    ${PROJECT_SOURCE_DIR}/../../src/block/GameObject.cpp
    ${PROJECT_SOURCE_DIR}/../../src/levels/GameLevel.cpp
    ${PROJECT_SOURCE_DIR}/../../src/Game.cpp
    ${PROJECT_SOURCE_DIR}/../../vendor/glad/src/glad.c
    ${PROJECT_SOURCE_DIR}/../../src/render/SpriteRenderer.cpp
)


set(LIB
    ${PROJECT_SOURCE_DIR}/../../vendor/glfw-3.3.7-wsl/lib/libglfw3.a
    GL m Xrandr  X11  pthread dl  
)   
add_executable(main
    ${SRC}
)

target_include_directories(main
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../../src/tools
        ${PROJECT_SOURCE_DIR}/../../src/render
        ${PROJECT_SOURCE_DIR}/../../src/block
        ${PROJECT_SOURCE_DIR}/../../src/levels
        ${PROJECT_SOURCE_DIR}/../../src
        ${PROJECT_SOURCE_DIR}/../../vendor/glad/include
        ${PROJECT_SOURCE_DIR}/../../vendor/glfw-3.3.7-wsl/include
        ${PROJECT_SOURCE_DIR}/../../vendor/glm
        ${PROJECT_SOURCE_DIR}/../../vendor/std_image
)

target_link_libraries(main
    PUBLIC
        ${LIB}
)

