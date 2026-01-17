#pragma once

#include <string>

enum class LogLevel {
	Debug,
	Warn,
	Error,
	Fatal,
};

namespace Log {
	void Init();
	void Quit();

	void SetLogLevel(LogLevel level);

	void Debug(const std::string &message);
	void Warn(const std::string &message);
	void Error(const std::string &message);
	void Fatal(const std::string &message);
}
