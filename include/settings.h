#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace Settings {
    void Init();

    fs::path GetDownloadFolderPath();
    void SetDownloadFolderPath(fs::path newPath);
}
