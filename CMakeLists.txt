
cmake_minimum_required (VERSION 2.8)
project (4K_displaywall_bench)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw-3.2)

include_directories( . glfw-3.2/deps glm png)

set(GLAD "glfw-3.2/deps/glad.c")

set(PNG "png/lodepng.cpp")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if (COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


add_executable (4K_displaywall_bench main.cpp Scene.cpp ${GLAD} ${PNG})

target_link_libraries(4K_displaywall_bench glfw GL)
