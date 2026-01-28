#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <variant>

namespace fs = std::filesystem;

using PathList = std::vector<fs::path>;

struct SDL_Process;

struct Process
{
    struct SendData
    {
        PathList archivePaths;
    };

    struct ReceiveData
    {
        float timerSeconds;
    };

    enum class Type
    {
        SEND,
        RECEIVE,
    } type;

    std::string ticket;
    std::variant<SendData, ReceiveData> data;
    SDL_Process* sdlProcess = nullptr;
    std::vector<std::string> command;
};
