//
// Created by Lucas on 15/02/2025.
//

#include "callback.h"

#include "globals.h"

#include <SDL3/SDL.h>

#include <cstdlib>
#include <iostream>
#include <queue>

void ChangeDownloadFolderCallback(void* userdata, const char* const* fileList, int) {
    AppState& app = *static_cast<AppState*>(userdata);

    if (fileList == nullptr) {
        std::cout << "ERROR: File list is null, " << SDL_GetError() << std::endl;
        return;
    }

    if (*fileList == nullptr) {
        return;
    }

    fs::path downloadPath = fileList[0];

    app.trashPaths.push_back(downloadPath);

    fs::current_path(downloadPath);
    app.downloadPath.swap(downloadPath);
}

void SendStuffCallback(void* userdata, const char * const *fileList, int filter) {
    AppState& app = *static_cast<AppState*>(userdata);

    if (fileList == nullptr) {
        std::cout << "ERROR: File list is null, " << SDL_GetError() << std::endl;
        return;
    }

    if (fileList[0] == nullptr) {
        std::cout << "File list is empty" << std::endl;
        return;
    }

    std::queue<fs::path> files;
    for (int i = 0; fileList[i] != nullptr; i++) {
        files.emplace(fileList[i]);
    }

    std::cout << app.downloadPath << std::endl;
    fs::current_path(app.downloadPath);

    for (auto& sendProcess : app.upload) {
        if (files.empty())
            break;
        if (sendProcess.sdlProcess != nullptr) continue;

        std::string str = app.binaryPath.string();
        char* sendMeCommand = SDL_iconv_utf8_locale(str.data());
        str = files.front().string();
        char* filePath = SDL_iconv_utf8_locale(str.data());

        // Use SDL_Properties to attribute buffers to outputs of process.
        SDL_PropertiesID props = SDL_CreateProperties();

        std::cout << sendMeCommand << " " << filePath << std::endl;

        const char* command[] = {
            sendMeCommand,
            "send",
            filePath,
            nullptr,
        };

        sendProcess.sdlProcess = SDL_CreateProcess(command, true);
        SDL_free(sendMeCommand);
        SDL_free(filePath);

        if (sendProcess.sdlProcess == nullptr) {
            std::cout << "Failed to create process" << std::endl;
            sendProcess.sdlProcess = nullptr;
            files.pop();
            continue;
        }

        sendProcess.filePath = files.front();
        sendProcess.loading = true;
        sendProcess.timer = 0.f;
        files.pop();
    }

    if (!files.empty()) {
        std::cout << "Not all files could be sent, check if you have 16 files already being sent" << std::endl;
    }
}
