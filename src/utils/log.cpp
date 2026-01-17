#include "utils/log.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "globals.h"
#include "SDL3/SDL_messagebox.h"

namespace fs = std::filesystem;

namespace Log {
	namespace {
		LogLevel currentLevel = LogLevel::Error;
		std::ofstream logFile;
	}

	void Init() {
		logFile.open(fs::current_path() / "log.txt");
		if (!logFile.is_open()) {
			std::exit(EXIT_FAILURE);
		}
	}

	void Quit() {
		logFile.close();
	}

	void SetLogLevel(const LogLevel level) {
		currentLevel = level;
	}

	void ShowFatalMessageBox(const std::string &message) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"A fatal error has occurred!",
			message.c_str(),
			globalContext.window
		);
	}

	void Message(const LogLevel level, const std::string &message) {
#ifdef DEBUG
		std::string finalMessage;
		if (level >= LogLevel::Debug) {
			if (currentLevel < level) return;

			finalMessage += "[DEBUG]: " + message;
		} else if (level >= LogLevel::Warn) {
			if (currentLevel < level) return;

			finalMessage += "[WARNING]: " + message;
		} else if (level >= LogLevel::Error) {
			if (currentLevel < level) return;
			finalMessage += "[ERROR]: " + message;
		} else if (level >= LogLevel::Fatal) {
			if (currentLevel < level) return;
			finalMessage += "[FATAL]: " + message;
		}

		if (logFile.is_open()) {
			logFile << finalMessage;
		}

		if (level < LogLevel::Error)
			std::cout << finalMessage << std::endl;
		else
			std::cerr << finalMessage << std::endl;
#endif
	}

	void Debug(const std::string &message) {
		Message(LogLevel::Debug, message);
	}

	void Warn(const std::string &message) {
		Message(LogLevel::Warn, message);
	}

	void Error(const std::string &message) {
		Message(LogLevel::Error, message);
	}

	void Fatal(const std::string &message) {
		Message(LogLevel::Fatal, message);
		ShowFatalMessageBox(message);
	}
}
