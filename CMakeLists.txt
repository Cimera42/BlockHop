cmake_minimum_required(VERSION 3.6)
project(BlockHop_CLion)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(libraries/include)

file(GLOB_RECURSE source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(SOURCE_FILES ${source} src/ecs/ecsManager.h src/ecs/escManager.cpp src/ecs/escLoader.cpp src/ecs/escLoader.h src/ecs/TestClass.cpp src/ecs/TestClass.h src/ecs/TestSystem.cpp src/ecs/TestSystem.h)
add_executable(BlockHop_CLion ${SOURCE_FILES})

find_package(OpenGL REQUIRED)

target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glew32.lib)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libassimp.dll.a)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glfw3dll.lib)
target_link_libraries(BlockHop_CLion ${OPENGL_LIBRARIES})