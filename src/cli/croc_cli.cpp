#include "transfer/croc_cli.h"

#include "globals.h"
#include "utils/log.h"
#include "uuid_v4.h"

#include <deque>
#include <random>

#include <SDL3/SDL_process.h>
#include <SDL3/SDL_events.h>

UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;

SendProcess CrocCLI::SendFiles(const PathList filePaths) {
    SendProcess newProcess = {};

#ifdef SDL_PLATFORM_WINDOWS
    binaryPath.concat(".exe");
#endif

    std::vector processArgs = {
        absolute(binaryPath).c_str(),
        "--yes",
        "--code"
    };

    std::string ticket = "SentYa_";
    const UUIDv4::UUID uuid = uuidGenerator.getUUID();
    ticket.append(uuid.str());
    processArgs.push_back(ticket.c_str());

    Log::Debug("Ticket Produced: " + ticket);

    for (auto &path: filePaths) {
        processArgs.push_back(absolute(path).c_str());
    }
    processArgs.push_back(nullptr);

    std::stringstream ss;
    for (const auto arg: processArgs) {
        if (arg == nullptr) continue;

        ss << arg << " ";
    }
    Log::Debug(ss.str());

    newProcess.sdlProcess = SDL_CreateProcess(processArgs.data(), true);
    if (newProcess.sdlProcess == nullptr) {
        Log::Error("Could not create process!" + std::string(SDL_GetError()));
    }
    newProcess.ticket = ticket;

    return newProcess;
}

SendProcess CrocCLI::SendFolder(fs::path folderPath) {
    SendProcess newProcess = {};

    return newProcess;
}

ReceiveProcess CrocCLI::ReceiveArchive(const std::string &ticket) {
    std::deque processArgs = {
        absolute(binaryPath).c_str(),
    };

#if defined(SDL_PLATFORM_LINUX) || defined(SDL_PLATFORM_APPLE)
    processArgs.push_front(("CROC_SECRET=" + ticket).c_str());
#else
    processArgs.push_back(ticket.c_str());
#endif
    processArgs.push_back(nullptr);

    ReceiveProcess newProcess = {

    };

    return newProcess;
}
