#pragma once

#include <chrono>

namespace diamond_engine {
class DeltaTimer {
public:
	DeltaTimer();

	float GetDeltaTimeSeconds() const;

	void Start();

	void End();
	
private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_end;
};
}
