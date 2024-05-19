#include <stdexcept>

#include <irrKlang.h>

#include "AudioDevice.h"

namespace diamond_engine
{
	AudioDevice::AudioDevice()
	{
		m_soundEngine = irrklang::createIrrKlangDevice();

		if (!m_soundEngine)
		{
			throw std::runtime_error("Could not create audio device");
		}
	}

	irrklang::ISoundSource* AudioDevice::loadSound(const std::string& file, bool preload /* = false */)
	{
		return m_soundEngine->addSoundSourceFromFile(file.c_str(), irrklang::ESM_AUTO_DETECT, preload);
	}

	void AudioDevice::unloadSound(irrklang::ISoundSource* source)
	{
		m_soundEngine->removeSoundSource(source);
	}

	void AudioDevice::playSound2D(irrklang::ISoundSource* source, bool loop /* = false */)
	{
		m_soundEngine->play2D(source, loop);
	}

	AudioDevice::~AudioDevice()
	{
		if (m_soundEngine)
		{
			m_soundEngine->removeAllSoundSources();
			m_soundEngine->drop();
		}
	}
}