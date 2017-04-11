cmake_minimum_required(VERSION 3.6)
project(BlockHop_CLion)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(libraries/include)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(BlockHop_CLion ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)

include_directories(${OPENAL_INCLUDE_DIR})

target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glew32.lib)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libassimp.dll.a)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glfw3dll.lib)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glfw3dll.lib)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libalut.dll.a)
target_link_libraries(BlockHop_CLion ${OPENGL_LIBRARIES})
target_link_libraries(BlockHop_CLion ${OPENAL_LIBRARY})
target_link_libraries(BlockHop_CLion "C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win64/EFX-Util_MT/EFX-Util.lib")