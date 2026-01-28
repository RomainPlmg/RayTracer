cmake_minimum_required(VERSION 3.10.0)
project(RayTracer VERSION 0.1.0 LANGUAGES C CXX)

# ============================================================================
# C++ Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Define executable
# ============================================================================
file(GLOB_RECURSE RT_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(RayTracer ${RT_SRC})
target_include_directories(RayTracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(RayTracer PRIVATE "-DASSETS_DIR=\"${CMAKE_SOURCE_DIR}/assets/\"")

# ============================================================================
# Dependencies
# ============================================================================
find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.3
)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.5-docking
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(imgui)

add_subdirectory(vendor)

# IMGUI doesn't have a CMakeLists.txt, so need to compile manually
file(GLOB IMGUI_SRC
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_sources(RayTracer PRIVATE ${IMGUI_SRC})
target_include_directories(RayTracer PRIVATE ${imgui_SOURCE_DIR} vendor/stb)
target_link_libraries(RayTracer PUBLIC OpenGL::GL glad glfw glm::glm)