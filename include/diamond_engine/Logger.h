#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

namespace diamond_engine {
class Logger {
public:
	enum class LogSeverity : unsigned {
		Info		= 0,
		Warning		= 1,
		Error		= 2,
		Critical	= 3
	};

	static const std::unordered_map<LogSeverity, std::string> kSeverityToStringMap;
	static const std::string kDateTimeFormatString;

	static std::string LogSeverityToString(LogSeverity logSeverity);

	static std::ostream& Log(std::ostream& stream, LogSeverity severity, const char* file, unsigned long long line);
};
}
