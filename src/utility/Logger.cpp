#include "utility/Logger.h"

#include <ctime>
#include <filesystem>
#include <vector>

namespace diamond_engine {
	/* static */ const std::unordered_map<Logger::LogSeverity, std::string> Logger::kSeverityToStringMap = {
		{ Logger::LogSeverity::Info,		"I" },
		{ Logger::LogSeverity::Warning,		"W" },
		{ Logger::LogSeverity::Error,		"E" },
		{ Logger::LogSeverity::Critical,	"C" }
	};

	/* static */ const std::string Logger::kDateTimeFormatString = "%d/%m/%Y|%H:%M:%S";

	/* static */ std::string Logger::LogSeverityToString(LogSeverity logSeverity) {
		if (kSeverityToStringMap.find(logSeverity) == kSeverityToStringMap.end()) {
			return "U";
		}

		return kSeverityToStringMap.at(logSeverity);
	}

	/* static */ std::ostream& Logger::Log(std::ostream& stream, LogSeverity severity, const char* file, unsigned long long line) {
		std::time_t currentTime = std::time(nullptr);

		return stream
			<< '[' << std::put_time(std::localtime(&currentTime), kDateTimeFormatString.c_str()) << ']'
			<< '[' << LogSeverityToString(severity) << ']'
			<< '[' << std::filesystem::path(file).filename().string()
			<< ':'
			<< line << "] ";
	}
}