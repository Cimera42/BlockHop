cmake_minimum_required(VERSION 3.6)
project(BlockHop)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(libraries/include)
# filepaths in header includes for reactphysics3d are based on itself being the root
# easier to just make it so than go through and fix them all
include_directories(libraries/include/reactphysics3d)

set(SOURCE_FILES src/main.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/logger.cpp src/logger.h)
set(SOURCE_FILES ${SOURCE_FILES} src/mesh.cpp src/mesh.h)
set(SOURCE_FILES ${SOURCE_FILES} src/window.cpp src/window.h)
set(SOURCE_FILES ${SOURCE_FILES} src/shader.cpp src/shader.h)
set(SOURCE_FILES ${SOURCE_FILES} src/openGLFunctions.cpp src/openGLFunctions.h)
set(SOURCE_FILES ${SOURCE_FILES} src/loadShader.cpp src/loadShader.h)
set(SOURCE_FILES ${SOURCE_FILES} src/boneMesh.cpp src/boneMesh.h)
set(SOURCE_FILES ${SOURCE_FILES} src/angle.h)

set(SOURCE_FILES ${SOURCE_FILES} src/ecs/component.cpp src/ecs/component.h)
set(SOURCE_FILES ${SOURCE_FILES} src/ecs/ecsLoader.cpp src/ecs/ecsLoader.h)
set(SOURCE_FILES ${SOURCE_FILES} src/ecs/ecsManager.cpp src/ecs/ecsManager.h)
set(SOURCE_FILES ${SOURCE_FILES} src/ecs/entity.cpp src/ecs/entity.h)
set(SOURCE_FILES ${SOURCE_FILES} src/ecs/system.cpp src/ecs/system.h)
set(SOURCE_FILES ${SOURCE_FILES} src/ecs/trigger.cpp src/ecs/trigger.h)

set(SOURCE_FILES ${SOURCE_FILES} src/systems/TestSystem.cpp src/systems/TestSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/EventSystem.cpp src/systems/EventSystem.cpp)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/TimeSystem.cpp src/systems/TimeSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/TextSystem.cpp src/systems/TextSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/AnimatedModelSystem.cpp src/systems/AnimatedModelSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/CameraSystem.cpp src/systems/CameraSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/MouseInputSystem.cpp src/systems/MouseInputSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/KeyboardInputSystem.cpp src/systems/KeyboardInputSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/MouseButtonSystem.cpp src/systems/MouseButtonSystem.h)
set(SOURCE_FILES ${SOURCE_FILES} src/systems/PhysicsSystem.cpp src/systems/PhysicsSystem.h)

set(SOURCE_FILES ${SOURCE_FILES} src/components/TestComponent.cpp src/components/TestComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/TimeoutComponent.cpp src/components/TimeoutComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/TransformComponent.cpp src/components/TransformComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/TextComponent.cpp src/components/TextComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/AnimatedModelComponent.cpp src/components/AnimatedModelComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/CameraComponent.cpp src/components/CameraComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/MouseControlComponent.cpp src/components/MouseControlComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/KeyboardControlComponent.cpp src/components/KeyboardControlComponent.h)
set(SOURCE_FILES ${SOURCE_FILES} src/components/PhysicsComponent.cpp src/components/PhysicsComponent.h)

set(SOURCE_FILES ${SOURCE_FILES} src/triggers/ClickedTrigger.cpp src/triggers/ClickedTrigger.h)

set(SOURCE_FILES ${SOURCE_FILES} src/loaders/imageLoader.cpp src/loaders/imageLoader.h)
set(SOURCE_FILES ${SOURCE_FILES} src/loaders/assetManager.cpp src/loaders/assetManager.h)
set(SOURCE_FILES ${SOURCE_FILES} src/loaders/assetLoader.cpp src/loaders/assetLoader.h)
set(SOURCE_FILES ${SOURCE_FILES} src/loaders/baseAsset.cpp src/loaders/baseAsset.h)
set(SOURCE_FILES ${SOURCE_FILES} src/loaders/modelAsset.cpp src/loaders/modelAsset.h)
set(SOURCE_FILES ${SOURCE_FILES} src/loaders/imageAsset.cpp src/loaders/imageAsset.h)
set(SOURCE_FILES ${SOURCE_FILES} src/loaders/modelLoader.cpp src/loaders/modelLoader.h)

add_executable(BlockHop ${SOURCE_FILES})

find_package(OpenGL REQUIRED)

target_link_libraries(BlockHop ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glew32.lib)
target_link_libraries(BlockHop ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libassimp.dll.a)
target_link_libraries(BlockHop ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glfw3dll.lib)
target_link_libraries(BlockHop ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libreactphysics3d.a)
target_link_libraries(BlockHop ${OPENGL_LIBRARIES})