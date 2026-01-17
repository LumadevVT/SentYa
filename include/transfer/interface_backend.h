#pragma once

#include "process/types.h"

#include <vector>

using PathList = std::vector<fs::path>;

class IBackend {
public:
    virtual ~IBackend() = default;

    virtual SendProcess SendFiles(PathList filePaths) = 0;
    virtual SendProcess SendFolder(fs::path folderPath) = 0;

    virtual ReceiveProcess ReceiveArchive(const std::string &ticket) = 0;
};
