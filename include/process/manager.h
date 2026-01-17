#pragma once

#include "process/types.h"

#include <string>
#include <vector>

using PathList = std::vector<fs::path>;

namespace ProcessManager {
    enum class Backend {
        CROC_CLI,
        SENDME_CLI,
        UNSUPPORTED
    };

    std::vector<std::string> GetAvailableBackends();
    std::string BackendToStr(Backend backend);

    void Init();
    void ChangeBackend(Backend backend);
    void Quit();

    void SendFiles(const PathList &filePaths);
    void SendFolder(const fs::path &folderPath);

    void ReceiveArchive(const std::string &ticket);
}
