//
// Created by Lucas on 15/02/2025.
//

#pragma once

#include <array>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_IOStream;
struct SDL_Process;

struct Process {
    fs::path filePath;
    std::string ticket;

    bool loading = true;
    SDL_Process* sdlProcess = nullptr;

    float timer = 0.f;
};

struct AppState {
    fs::path binaryPath = fs::current_path() / "binaries" / "sendme.exe";
    fs::path downloadPath = fs::current_path();

    std::array<Process, 16> upload = {};
    std::array<Process, 16> download = {};

    std::vector<fs::path> trashPaths = {};
};

struct Context {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    AppState app;
};