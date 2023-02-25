#include <iostream>

#include "LogManager.h"

namespace diamond_engine {
	LogManager& LogManager::getInstance() {
		static LogManager logManagerInstance;

		return logManagerInstance;
	}

	void LogManager::enqueueLogMessage(const std::string& message, Logger::LogSeverity severity, const char* file, unsigned long long line) {
		std::unique_lock<std::mutex> lock(logMutex);
		logMessageQueue.push_back(LogMessage{ message, file, line, severity });
		shouldLog.store(true);
		lock.unlock();

		logSemaphore.notify_one();
	}

	void LogManager::Stop() {
		shouldStop.store(true);

		logSemaphore.notify_one();

		if (workerThread.joinable()) {
			workerThread.join();
		}
	}

	LogManager::~LogManager() {
		Stop();
	}

	LogManager::LogManager() {
		workerThread = std::thread(&LogManager::dequeueLogMessages, this);
	}

	void LogManager::dequeueLogMessages() {
		while (!shouldStop.load()) {
			std::unique_lock<std::mutex> lock(logMutex);

			logSemaphore.wait(lock, [&]() { return shouldStop || shouldLog; });

			if (shouldLog && !logMessageQueue.empty()) {
				const LogMessage& logMessage = logMessageQueue.front();

				Logger::Log(std::cout, logMessage.severity, logMessage.file, logMessage.line)
					<< logMessage.message << std::endl;

				logMessageQueue.pop_front();
			}
		}
	}
}