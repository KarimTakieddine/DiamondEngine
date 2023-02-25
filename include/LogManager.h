#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "Logger.h"

namespace diamond_engine {
class LogManager {
public:
	struct LogMessage {
		std::string message;
		const char* file;
		unsigned long long line;
		Logger::LogSeverity severity;
	};

	static LogManager& getInstance();

	void EnqueueLogMessage(
		const std::string& message,
		Logger::LogSeverity severity,
		const char* file,
		unsigned long long line);

	void Stop();

	~LogManager();

private:
	LogManager();

	void DequeueLogMessages();

	std::deque<LogMessage> logMessageQueue;
	std::thread workerThread;
	std::mutex logMutex;
	std::condition_variable logSemaphore;
	std::atomic<bool> shouldStop{ false };
	std::atomic<bool> shouldLog{ false };
};
}

#define LOG_INFO(msg)		diamond_engine::LogManager::getInstance().EnqueueLogMessage(msg, diamond_engine::Logger::LogSeverity::Info,		__FILE__, __LINE__);
#define LOG_WARNING(msg)	diamond_engine::LogManager::getInstance().EnqueueLogMessage(msg, diamond_engine::Logger::LogSeverity::Warning,	__FILE__, __LINE__);
#define LOG_ERROR(msg)		diamond_engine::LogManager::getInstance().EnqueueLogMessage(msg, diamond_engine::Logger::LogSeverity::Error,	__FILE__, __LINE__);
#define LOG_CRITICAL(msg)	diamond_engine::LogManager::getInstance().EnqueueLogMessage(msg, diamond_engine::Logger::LogSeverity::Critical, __FILE__, __LINE__);
