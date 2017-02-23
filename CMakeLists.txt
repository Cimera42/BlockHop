cmake_minimum_required(VERSION 3.6)
project(BlockHop_CLion)

set(CMAKE_CXX_STANDARD 11)

include_directories(libraries/include)

file(GLOB source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h src/*.cpp)
set(SOURCE_FILES ${source})
add_executable(BlockHop_CLion ${SOURCE_FILES})

target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libglew32.dll.a)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/glfw3dll.a)
target_link_libraries(BlockHop_CLion glu32)
target_link_libraries(BlockHop_CLion opengl32)
target_link_libraries(BlockHop_CLion ${CMAKE_CURRENT_SOURCE_DIR}/libraries/lib/libassimp.dll.a)