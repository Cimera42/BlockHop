cmake_minimum_required(VERSION 3.6)
project(BlockHop)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(libraries/include)

set(files src/main.cpp src/main.h)
set(files ${files} src/logger.h)
set(files ${files} src/mesh.cpp src/mesh.h)
set(files ${files} src/window.cpp src/window.h)
set(files ${files} src/texture.cpp src/texture.h)
set(files ${files} src/shader.cpp src/shader.h)
set(files ${files} src/camera.cpp src/camera.h)
set(files ${files} src/shader.cpp src/shader.h)
set(files ${files} src/openGLFunctions.cpp src/openGLFunctions.h)
set(files ${files} src/loadShader.cpp src/loadShader.h)
set(files ${files} src/boneMesh.cpp src/boneMesh.h)
set(files ${files} src/boneModel.cpp src/boneModel.h)
set(files ${files} src/angle.h)

set(files ${files} src/ecs/component.cpp src/ecs/component.h)
set(files ${files} src/ecs/ecsLoader.cpp src/ecs/ecsLoader.h)
set(files ${files} src/ecs/ecsManager.cpp src/ecs/ecsManager.h)
set(files ${files} src/ecs/entity.cpp src/ecs/entity.h)
set(files ${files} src/ecs/system.cpp src/ecs/system.h)

set(files ${files} src/systems/TestSystem.cpp src/systems/TestSystem.h)
set(files ${files} src/systems/TextSystem.cpp src/systems/TextSystem.h)

set(files ${files} src/components/TestComponent.cpp src/components/TestComponent.h)
set(files ${files} src/components/TransformComponent.cpp src/components/TransformComponent.h)
set(files ${files} src/components/TextComponent.cpp src/components/TextComponent.h)
set(files ${files} src/components/BoneModelComponent.cpp src/components/BoneModelComponent.h)

file(GLOB_RECURSE SOURCE_FILES ${files})
add_executable(BlockHop ${SOURCE_FILES})

find_package(OpenGL REQUIRED)

target_link_libraries(BlockHop ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glew32.lib)
target_link_libraries(BlockHop ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libassimp.dll.a)
target_link_libraries(BlockHop ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glfw3dll.lib)
target_link_libraries(BlockHop ${OPENGL_LIBRARIES})