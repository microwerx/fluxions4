cmake_minimum_required(VERSION 3.16)
project(fluxions4)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if (APPLE)
   include_directories(/usr/local/include)
   link_directories(/usr/local/lib)
#   set(STD_CXX_FS c++fs)
endif()
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../imgui/)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME}
    src/fluxions4_simple_shapes.cpp
    src/fluxions4_vulkan_descriptor_set.cpp
    src/fluxions4_vulkan_render_pass.cpp
    src/test_fluxions4.cpp
    src/fluxions4_superquadrics.cpp
    src/fluxions4_vulkan_image.cpp
    src/fluxions4_vulkan_vertex.cpp
    src/fluxions4_vulkan_buffer.cpp
    src/fluxions4_vulkan_mesh.cpp
    src/hatchetfish_log.cpp
    src/fluxions4_vulkan_context.cpp
    src/fluxions4_vulkan_pipeline.cpp
    src/hatchetfish_stopwatch.cpp)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} SDL2)
