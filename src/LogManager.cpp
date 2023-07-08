#include <iostream>

#include "LogManager.h"

namespace diamond_engine {
	LogManager& LogManager::getInstance() {
		static LogManager logManagerInstance;

		return logManagerInstance;
	}

	void LogManager::EnqueueLogMessage(const std::string& message, Logger::LogSeverity severity, const char* file, unsigned long long line) {
		std::unique_lock<std::mutex> lock(logMutex);
		logMessageQueue.push_back(LogMessage{ message, file, line, severity });
		lock.unlock();

		shouldLog.store(true, std::memory_order_seq_cst);

		logSemaphore.notify_one();
	}

	void LogManager::Stop() {
		shouldStop.store(true, std::memory_order_seq_cst);

		logSemaphore.notify_one();

		if (workerThread.joinable()) {
			workerThread.join();
		}
	}

	LogManager::~LogManager() {
		Stop();
	}

	LogManager::LogManager() {
		workerThread = std::thread(&LogManager::DequeueLogMessages, this);
	}

	void LogManager::DequeueLogMessages() {
		while (!shouldStop.load()) {
			std::unique_lock<std::mutex> lock(logMutex);

			logSemaphore.wait(lock, [&]() { return shouldStop.load(std::memory_order_seq_cst) || shouldLog.load(std::memory_order_seq_cst); });

			if (shouldLog.load(std::memory_order_seq_cst) && !logMessageQueue.empty()) {
				const LogMessage logMessage = logMessageQueue.front();
				logMessageQueue.pop_front();
				lock.unlock();

				Logger::Log(std::cout, logMessage.severity, logMessage.file, logMessage.line)
					<< logMessage.message << std::endl;

				shouldLog.store(false, std::memory_order_seq_cst);
			}
		}
	}
}