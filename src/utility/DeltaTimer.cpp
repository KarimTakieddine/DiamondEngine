#include "utility/DeltaTimer.h"

namespace diamond_engine {
	DeltaTimer::DeltaTimer() :
	m_start(std::chrono::steady_clock::now()),
	m_end(std::chrono::steady_clock::now()) { }

	float DeltaTimer::GetDeltaTimeSeconds() const {
		return m_end > m_start ?
			std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count() * 0.001f :
			0.0f;
	}

	void DeltaTimer::Start() {
		m_start = std::chrono::steady_clock::now();
	}

	void DeltaTimer::End() {
		m_end = std::chrono::steady_clock::now();
	}
}