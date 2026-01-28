#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace Settings
{
    bool Init();
    void SaveToDisk();

    fs::path& GetDownloadFolderPath();
    fs::path& GetConfigFolderPath();
    void SetDownloadFolderPath(fs::path newPath);
}
