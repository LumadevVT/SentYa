cmake_minimum_required(VERSION 3.25)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)

project(SentYa)

include(cmake/GetBinaries.cmake)

add_subdirectory(vendor EXCLUDE_FROM_ALL)

add_executable(
        SentYa
        src/main.cpp
        src/callback.cpp
        src/utils/log.cpp
        src/utils/time.cpp
        src/init_app.cpp
        include/context.h
        include/process/types.h
        include/window.h
        src/cli/croc_cli.cpp
        include/transfer/croc_cli.h
        include/gui/events.h
        include/gui/tabs.h
        include/gui.h
        src/gui/tabs.cpp
        src/gui.cpp
        include/transfer/croc_cli.h
        src/gui/events.cpp
        include/settings.h
        src/settings.cpp
        include/process/manager.h
        src/process/manager.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        SDL3::SDL3
        DearImGui::SDL3
        uuid_v4
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else ()
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=0)
endif ()

if (WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND_EXPAND_LISTS
    )
endif ()
