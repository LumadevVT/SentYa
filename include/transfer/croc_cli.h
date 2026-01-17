#pragma once

#include "interface_backend.h"

class CrocCLI final : public IBackend {
    fs::path binaryPath = fs::current_path() / "binaries" / "croc";

public:
    CrocCLI() = default;

    SendProcess SendFiles(PathList filePaths) override;
    SendProcess SendFolder(fs::path folderPath) override;

    ReceiveProcess ReceiveArchive(const std::string &ticket) override;
};
