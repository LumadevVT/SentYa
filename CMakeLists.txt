cmake_minimum_required(VERSION 3.22)

project(SentYa)

add_subdirectory(vendor EXCLUDE_FROM_ALL)

include(FetchContent)

add_executable(
    SentYa
    src/main.cpp
    src/callback.cpp
    include/callback.h
    include/globals.h
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE "include")
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    DearImGui::SDL3
)

set(SENDME_BINARY_VERSION "v0.24.0")

set(SENDME_BINARY_EXT "tar.gz")
set(OS ${CMAKE_SYSTEM_NAME})

if (APPLE)
    set(OS "MacOS")
elseif (CYGWIN OR WIN32)
    set(OS "Windows")
    set(SENDME_BINARY_EXT "zip")
endif()

string(TOLOWER ${OS} SENDME_BINARY_OS)

set(SENDME_URL "https://github.com/n0-computer/sendme/releases/download")
fetchcontent_declare(
    SendMeBinary
    URL "${SENDME_URL}/${SENDME_BINARY_VERSION}/sendme-${SENDME_BINARY_VERSION}-${SENDME_BINARY_OS}-x86_64.${SENDME_BINARY_EXT}"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/binaries/"
    DOWNLOAD_EXTRACT_TIMESTAMP true
)

fetchcontent_makeavailable(SendMeBinary)

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif ()