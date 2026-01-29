#include "utils/log.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "globals.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_messagebox.h"

namespace fs = std::filesystem;

namespace Log
{
	namespace
	{
		std::ofstream logFile;
	}

	static void LogOutputCallback(void*, int category, SDL_LogPriority priority, const char* message)
	{
		switch (priority)
		{
		// case SDL_LOG_PRIORITY_CRITICAL:
		//
		// case SDL_LOG_PRIORITY_ERROR:
		// case SDL_LOG_PRIORITY_WARN:
		// case SDL_LOG_PRIORITY_INFO:
		default:
			break;
		}
	}

	void Init()
	{
		logFile.open(fs::current_path() / "log.txt");
		if (!logFile.is_open())
		{
			std::exit(EXIT_FAILURE);
		}
		// SDL_SetLogOutputFunction(LogOutputCallback, nullptr);
	}

	void Quit()
	{
		logFile.close();
	}

	void SetLogLevel(const LogLevel level)
	{
	}

	void ShowFatalMessageBox(const std::string& message)
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"A fatal error has occurred!",
			message.c_str(),
			globalContext.window
		);
	}

	void Debug(const std::string& message)
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
	}

	void Warn(const std::string& message)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
	}

	void Error(const std::string& message)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
	}

	void Fatal(const std::string& message)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", message.c_str());
		ShowFatalMessageBox(message);
	}
}
