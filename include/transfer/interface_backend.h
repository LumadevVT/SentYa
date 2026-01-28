#pragma once

#include "process/types.h"

#include <vector>

class IBackend {
public:
    virtual ~IBackend() = default;

    virtual Process SendFiles(PathList filePaths) = 0;
    virtual Process SendFiles(PathList filePaths, const std::string& oldTicket) = 0;

    virtual Process SendFolder(fs::path folderPath) = 0;
    virtual Process SendFolder(fs::path folderPath, const std::string& oldTicket) = 0;

    virtual Process ReceiveArchive(const std::string &ticket) = 0;

    virtual void OnProcessFinish(Process& process) = 0;
    virtual void UpdateProcess(Process& process) = 0;

};
