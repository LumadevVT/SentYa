#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct SDL_Process;

struct Process {
    std::string ticket;
    SDL_Process *sdlProcess = nullptr;

    bool IsWorking() {
        return sdlProcess != nullptr;
    }
};

struct SendProcess final : Process {
    std::string archivePath;
};

struct ReceiveProcess final : Process {
    float timerSeconds = 0.f;
};
