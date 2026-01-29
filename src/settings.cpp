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
            fs::create_directory(fs::temp_directory_path() / "SentYa");
            prefs.path = fs::temp_directory_path() / "SentYa";
        }
        else
        {
            prefs.path.assign(prefPath);

            SDL_free(prefPath);
        }

        const char* downloadFolder = SDL_GetUserFolder(SDL_FOLDER_DOWNLOADS);
        if (downloadFolder != nullptr)
        {
            prefs.downloadFolder = fs::path(downloadFolder);
            return;
        }

        Log::Warn("Could not load download folder!");
        fs::create_directory(fs::temp_directory_path() / "SentYa" / "Downloads");
        prefs.downloadFolder = fs::temp_directory_path() / "SentYa" / "Downloads";
    }


    bool LoadPrefsFromDisk()
    {
        char* prefPath = SDL_GetPrefPath(ORGANIZATION, APP_NAME);
        if (prefPath == nullptr)
        {
            Log::Warn("Could not load preferences path from SDL!");
            return false;
        }
        prefs.path.assign(prefPath);
        SDL_free(prefPath);

        std::ifstream preferencesFile;
        preferencesFile.open(prefs.path / SETTINGS_FILENAME);
        if (!preferencesFile.is_open())
        {
            Log::Warn("Could not open preferences file!");
            return false;
        }

        try
        {
            json data = json::parse(preferencesFile);
            Log::Debug(data.dump(4));
            prefs.downloadFolder = data.at("downloadFolder").get<std::string>();

            preferencesFile.close();
            return true;
        }
        catch (const std::exception& e)
        {
            Log::Error(std::string("Error loading json from preferences: ") + e.what());
            preferencesFile.close();
            return false;
        }
    }

    bool Init()
    {
        if (!LoadPrefsFromDisk())
            LoadDefaultPreferences();
        return true;
    }

    void SaveToDisk()
    {
        Log::Debug((prefs.path / SETTINGS_FILENAME).string());

        std::ofstream preferencesFile;
        preferencesFile.open(prefs.path / SETTINGS_FILENAME);
        if (!preferencesFile.is_open()) return;


        json jsonData;
        jsonData.emplace("downloadFolder", prefs.downloadFolder);

        preferencesFile << std::setw(4) << jsonData << std::endl;
        preferencesFile.close();
    }

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
