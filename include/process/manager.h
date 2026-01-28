#pragma once

#include <list>

#include "process/types.h"

#include <string>
#include <vector>

using PathList = std::vector<fs::path>;

namespace ProcessManager {
    enum class Backend {
        CROC_CLI,
        // SENDME_CLI,
        // UNSUPPORTED
    };

    std::vector<Backend> GetAvailableBackends();
    const char* BackendToStr(Backend backend);

    Backend GetCurrentBackend();

    const std::list<Process>& GetActiveProcesses();

    void Init();
    void Update();
    void ChangeBackend(Backend backend);
    void Quit();

    void SendFiles(PathList filePaths);
    void SendFolder(fs::path folderPath);

    void ReceiveArchive(const std::string& ticket);

    void CloseProcess(const std::string &ticket);
}
