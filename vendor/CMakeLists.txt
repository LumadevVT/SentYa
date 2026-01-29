cmake_minimum_required(VERSION 3.25)

add_subdirectory(SDL3 EXCLUDE_FROM_ALL)
add_subdirectory(uuid_v4 EXCLUDE_FROM_ALL)
add_subdirectory(nlohmann_json EXCLUDE_FROM_ALL)

project(DearImGui)

set(DEAR_IMGUI_PATH dear_imgui)

add_library(
        DearImGui STATIC
        ${DEAR_IMGUI_PATH}/imgui.cpp
        ${DEAR_IMGUI_PATH}/imgui_draw.cpp
        ${DEAR_IMGUI_PATH}/imgui_tables.cpp
        ${DEAR_IMGUI_PATH}/imgui_widgets.cpp
        ${DEAR_IMGUI_PATH}/misc/cpp/imgui_stdlib.cpp
)
target_compile_features(DearImGui PRIVATE cxx_std_11)
target_include_directories(
        DearImGui PUBLIC
        ${DEAR_IMGUI_PATH}
        ${DEAR_IMGUI_PATH}/backends
        ${DEAR_IMGUI_PATH}/misc/cpp
)


add_library(
        DearImGuiSDL3 STATIC
        ${DEAR_IMGUI_PATH}/backends/imgui_impl_sdl3.cpp
        ${DEAR_IMGUI_PATH}/backends/imgui_impl_sdlrenderer3.cpp
        ${DEAR_IMGUI_PATH}/backends/imgui_impl_sdlgpu3.cpp
)
target_compile_features(DearImGuiSDL3 PRIVATE cxx_std_11)
target_link_libraries(DearImGuiSDL3 PUBLIC DearImGui PRIVATE SDL3::SDL3)
add_library(DearImGui::SDL3 ALIAS DearImGuiSDL3)
