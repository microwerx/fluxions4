cmake_minimum_required(VERSION 3.16)
project(fluxions4)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if (APPLE)
   include_directories(/usr/local/include)
   link_directories(/usr/local/lib)
endif()

add_subdirectory(hatchetfish)
include_directories(hatchetfish/include)
add_subdirectory(fluxions-gte)
include_directories(fluxions-gte/include)

include_directories(imgui)
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

execute_process(
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
  COMMAND make rebuild
)

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
    src/fluxions4_vulkan_context.cpp
    src/fluxions4_vulkan_pipeline.cpp
    src/imgui_main.cpp
    src/imgui_impl_sdl.cpp
    src/imgui_impl_vulkan.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    )

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} SDL2main)
target_link_libraries(${PROJECT_NAME} hatchetfish)
target_link_libraries(${PROJECT_NAME} fluxions-gte)
