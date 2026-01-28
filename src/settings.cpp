#include "settings.h"

#include "globals.h"
#include <SDL3/SDL.h>

#include <utility>
#include <fstream>

#include <fstream>
#include <nlohmann/json.hpp>

#include "utils/log.h"
using json = nlohmann::json;

namespace Settings
{
    namespace
    {
        // struct SavePreferences {
        //     size_t downloadPathSize = 0;
        //     const char *downloadPath = nullptr;
        // };

        struct Preferences
        {
            fs::path path;
            fs::path downloadFolder;
        } prefs;

        constexpr char SETTINGS_FILENAME[] = "SentYa_Settings.json";
        constexpr char ORGANIZATION[] = "LumadevVT";
        constexpr char APP_NAME[] = "SentYa";
    }

    void LoadDefaultPreferences()
    {
        char* prefPath = SDL_GetPrefPath(ORGANIZATION, APP_NAME);
        if (prefPath == nullptr)
        {
            Log::Warn("Could not load preferences folder!");
            fs::create_directory(fs::temp_directory_path() / "SentYa");
            prefs.path = fs::temp_directory_path() / "SentYa";
        }
        else prefs.path = prefPath;
        SDL_free(prefPath);

        const char* downloadFolder = SDL_GetUserFolder(SDL_FOLDER_DOWNLOADS);
        if (downloadFolder == nullptr)
        {
            Log::Warn("Could not load download folder!");
            fs::create_directory(fs::temp_directory_path() / "SentYa" / "Downloads");
            prefs.downloadFolder = fs::temp_directory_path() / "SentYa" / "Downloads";
        }
        else prefs.downloadFolder = downloadFolder;
    }


    bool LoadPrefsFromDisk()
    {
        const char* prefPath = SDL_GetPrefPath(ORGANIZATION, APP_NAME);
        if (prefPath == nullptr)
        {
            Log::Warn("Could not load preferences path from SDL!");
            return false;
        }

        try
        {
            std::ifstream preferencesFile(fs::path(prefPath) / SETTINGS_FILENAME);
            json data = json::parse(preferencesFile);

            prefs = {
                .path = prefPath,
                .downloadFolder = data["downloadFolder"].get<std::string>(),
            };
            return true;
        }
        catch (const std::exception& e)
        {
            Log::Error(e.what());
            return false;
        }
    }

    bool Init()
    {
        if (!LoadPrefsFromDisk()) LoadDefaultPreferences();

        return true;
    }

    void SaveToDisk()
    {
        std::ofstream preferencesFile;
        preferencesFile.open(prefs.path / SETTINGS_FILENAME);
        if (!preferencesFile.is_open()) return;

        json jsonData;
        jsonData["downloadFolder"] = prefs.downloadFolder.native();

        preferencesFile << std::setw(4) << jsonData << std::endl;
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

    fs::path& GetDownloadFolderPath()
    {
        return prefs.downloadFolder;
    }

    fs::path& GetConfigFolderPath()
    {
        return prefs.path;
    }

    void SetDownloadFolderPath(fs::path newPath)
    {
        if (!is_directory(newPath)) return;

        prefs.downloadFolder.swap(newPath);
    }
}
