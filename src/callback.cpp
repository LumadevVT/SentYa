#include "callback.h"

#include "settings.h"
#include "utils/log.h"
#include "process/manager.h"

#include <string>
#include <SDL3/SDL.h>

using namespace std::string_literals;

void ChangeDownloadFolderCallback(void* userdata, const char* const * fileList, int)
{
    if (fileList == nullptr)
    {
        Log::Error("File list is null when changing download folder. REASON: "s + SDL_GetError());
        return;
    }

    if (*fileList == nullptr)
    {
        Log::Debug("No new folder was selected or user canceled action.");
        return;
    }

    const fs::path downloadPath = fileList[0];

    Settings::SetDownloadFolderPath(downloadPath);
}

void SendFilesCallback(void* userdata, const char* const * fileList, int filter)
{
    PathList paths;
    if (fileList == nullptr)
    {
        Log::Error("File list is null when changing download folder. REASON: "s + SDL_GetError());
        return;
    }

    if (fileList[0] == nullptr)
    {
        Log::Debug("No new folder was selected or user canceled action.");
        return;
    }

    for (int i = 0; fileList[i] != nullptr; i++)
    {
        paths.emplace_back(fileList[i]);
    }

    ProcessManager::SendFiles(paths);
}

void SendFoldersCallback(void* userdata, const char* const * fileList, int filter)
{
    if (fileList == nullptr)
    {
        Log::Error("File list is null when changing download folder. REASON: "s + SDL_GetError());
        return;
    }

    if (fileList[0] == nullptr)
    {
        Log::Debug("No new folder was selected or user canceled action.");
        return;
    }

    ProcessManager::SendFolder(fileList[0]);
}
