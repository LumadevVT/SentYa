cmake_minimum_required(VERSION 3.25)

add_subdirectory(vendor EXCLUDE_FROM_ALL)

project(SentYa)

add_executable(
        SentYa
        src/main.cpp
        src/callback.cpp
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
        src/gui/events.cpp
        include/settings.h
        src/settings.cpp
        include/process/manager.h
        src/process/manager.cpp
        include/p2p/p2p.h
        src/p2p/p2p.cpp
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_sources(SentYa PRIVATE src/cli/croc_windows.cpp)
else ()
    target_sources(SentYa PRIVATE src/cli/croc_unix.cpp)
endif ()

target_compile_features(SentYa PRIVATE cxx_std_20)
target_include_directories(SentYa PRIVATE "include")
target_link_libraries(
        SentYa PRIVATE
        DearImGui::SDL3
        SDL3::SDL3
        rats
        uuid_v4
        nlohmann_json
)


if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_custom_command(
            TARGET SentYa POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_RUNTIME_DLLS:SentYa>
            $<TARGET_FILE_DIR:SentYa>
            COMMAND_EXPAND_LISTS
    )
endif ()
