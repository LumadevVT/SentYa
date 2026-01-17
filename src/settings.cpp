#include "settings.h"

#include "globals.h"
#include <SDL3/SDL.h>

#include <utility>
#include <fstream>

namespace Settings {
    namespace {
        // struct SavePreferences {
        //     size_t downloadPathSize = 0;
        //     const char *downloadPath = nullptr;
        // };

        struct Preferences {
            fs::path downloadPath;
        } prefs;

        fs::path prefsPath;
        constexpr char ORGANIZATION[] = "Lumadev";
        constexpr char APP_NAME[] = "SentYa";
    }

    void Init() {
        char *sdlPrefPath = SDL_GetPrefPath(ORGANIZATION, APP_NAME);
        if (sdlPrefPath == nullptr) {
            prefsPath = fs::temp_directory_path();
        } else {
            prefsPath = sdlPrefPath;
            SDL_free(sdlPrefPath);
        }

        const char *tempStr = SDL_GetUserFolder(SDL_FOLDER_DOWNLOADS);
        if (tempStr == nullptr) {
            prefs.downloadPath = fs::temp_directory_path() / "SentYa";
            if (!exists(prefs.downloadPath)) {
                fs::create_directory(prefs.downloadPath);
            }
        } else {
            prefs.downloadPath = tempStr;
        }
    }

    // TODO: Maybe save preferences for SentYa in future updates
    // void SaveToDisk() {
    //     static_assert(std::is_standard_layout_v<SavePreferences> == true);
    //     std::string filePath = prefsPath.append("SentYa.config").string();
    //
    //     SavePreferences savePrefs = {};
    //     savePrefs.downloadPath = prefs.downloadPath.native().c_str();
    //     savePrefs.downloadPathSize = prefs.downloadPath.native().length();
    //
    //     SDL_SaveFile(filePath.c_str(), &savePrefs, sizeof(savePrefs));
    // }

    fs::path GetDownloadFolderPath() {
        return prefs.downloadPath;
    }

    void SetDownloadFolderPath(fs::path newPath) {
        if (!is_directory(newPath)) return;

        prefs.downloadPath.swap(newPath);
    }
}
