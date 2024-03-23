#include <irrKlang.h>

#include "AudioEngine.h"

namespace diamond_engine
{
	/* static */ std::shared_ptr<AudioEngine>& AudioEngine::instance()
	{
		static std::shared_ptr<AudioEngine> audioEngineSharedInstance(new AudioEngine());
		return audioEngineSharedInstance;
	}

	AudioEngine::AudioEngine() : m_audioLoader(std::make_unique<AudioLoader>()) { }

	void AudioEngine::initialize(const std::string& audioDirectory)
	{
		m_audioDevice = std::make_unique<AudioDevice>();
		m_audioLoader->load(audioDirectory, m_audioDevice.get());
	}

	void AudioEngine::playSound2D(const std::string& name, bool loop /* = false */)
	{
		auto soundSource = m_audioLoader->loadSound({ name }, m_audioDevice.get());
		m_audioDevice->playSound2D(soundSource, loop);
	}
}