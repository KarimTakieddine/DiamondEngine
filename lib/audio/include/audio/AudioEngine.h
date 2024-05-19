#pragma once

#include <memory>

#include "AudioDevice.h"
#include "AudioLoader.h"

namespace irrklang { class ISoundEngine; }

namespace diamond_engine
{
	class AudioEngine
	{
	public:
		static std::shared_ptr<AudioEngine>& getInstance();

		void initialize(const std::string& audioDirectory);

		void playSound2D(const std::string& name, bool loop = false);

	private:
		AudioEngine();

		std::unique_ptr<AudioDevice> m_audioDevice{ nullptr };
		std::unique_ptr<AudioLoader> m_audioLoader{ nullptr };
	};
}
