#include "callback.h"

#include <SDL3/SDL.h>

#include <queue>

#include "settings.h"
#include "utils/log.h"
#include "process/manager.h"


void ChangeDownloadFolderCallback(void *userdata, const char *const *fileList, int) {
    if (fileList == nullptr) {
        Log::Error("File list is null when changing download folder. REASON: " + std::string(SDL_GetError()));
        return;
    }

    if (*fileList == nullptr) {
        Log::Debug("No new folder was selected or user canceled action.");
        return;
    }

    const fs::path downloadPath = fileList[0];

    Settings::SetDownloadFolderPath(downloadPath);
}

void SendFilesCallback(void *userdata, const char *const *fileList, int filter) {
    PathList paths;
    if (fileList == nullptr) {
        Log::Error("File list is null when changing download folder. REASON: " + std::string(SDL_GetError()));
        return;
    }

    if (fileList[0] == nullptr) {
        Log::Debug("No new folder was selected or user canceled action.");
        return;
    }

    for (int i = 0; fileList[i] != nullptr; i++) {
        paths.emplace_back(fileList[i]);

    }

    ProcessManager::SendFiles(paths);
}

void SendFoldersCallback(void *userdata, const char *const *fileList, int filter) {
    if (fileList == nullptr) {
        Log::Error("File list is null when changing download folder. REASON: " + std::string(SDL_GetError()));
        return;
    }

    if (fileList[0] == nullptr) {
        Log::Debug("No new folder was selected or user canceled action.");
        return;
    }

    ProcessManager::SendFolder(fileList[0]);
}
