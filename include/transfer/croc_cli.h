#pragma once

#include "interface_backend.h"

class CrocCLI final : public IBackend
{
    const fs::path binariesPath = fs::current_path() / "binaries";

public:
    CrocCLI() = default;
    ~CrocCLI() override = default;

    Process SendFiles(PathList filePaths) override;
    Process SendFiles(PathList filePaths, const std::string& oldTicket) override;

    Process SendFolder(fs::path folderPath) override;
    Process SendFolder(fs::path folderPath, const std::string& oldTicket) override;

    Process ReceiveArchive(const std::string& ticket) override;

    void OnProcessFinish(Process& process) override;
    void UpdateProcess(Process& process) override;

private:
    [[nodiscard]] std::vector<std::string> CreateSendCommand(const PathList& paths, const std::string& ticket) const;
    [[nodiscard]] std::vector<std::string> CreateReceiveCommand(const std::string& ticket) const;
};
