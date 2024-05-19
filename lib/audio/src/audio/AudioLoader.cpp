#include <filesystem>
#include <stdexcept>

#include <irrKlang.h>

#include "AudioDevice.h"
#include "AudioLoader.h"
#include "SoundConfigParser.h"

namespace diamond_engine
{
	/* static */ const std::string AudioLoader::kAudioMetadataFilename = "audioMetadata.xml";

	void AudioLoader::load(const std::string& rootDirectory, AudioDevice* audioDevice)
	{
		const std::filesystem::path rootDirectoryPath(rootDirectory);

		if (!std::filesystem::is_directory(rootDirectoryPath))
		{
			throw std::runtime_error("AudioLoader - Cannot load from: " + rootDirectory + " as it is not a directory");
		}

		const std::filesystem::path metadataPath = rootDirectoryPath / kAudioMetadataFilename;

		if (!std::filesystem::exists(metadataPath))
		{
			throw std::runtime_error("AudioLoader - Cannot load metadata from: " + metadataPath.string());
		}

		auto soundConfigList = SoundConfigParser::parse(metadataPath.string());

		m_soundSourceMap.clear();
		for (auto& soundConfig : soundConfigList)
		{
			std::filesystem::path soundPath(soundConfig.path);

			if (!soundPath.is_absolute())
			{
				soundPath = rootDirectoryPath / soundPath;
				
				soundPath.make_preferred();

				soundConfig.path = soundPath.string();
			}

			if (!std::filesystem::exists(soundPath) || !std::filesystem::is_regular_file(soundPath))
			{
				throw std::runtime_error("AudioLoader - Cannot find referenced sound file: " + soundPath.string());
			}

			loadSound(soundConfig, audioDevice);
		}
	}

	irrklang::ISoundSource* AudioLoader::loadSound(const SoundConfig& soundConfig, AudioDevice* audioDevice, bool forceReload /* = false */)
	{
		const std::string& soundName = soundConfig.name;

		if (m_soundSourceMap.find(soundName) != m_soundSourceMap.cend())
		{
			if (forceReload)
			{
				audioDevice->unloadSound(m_soundSourceMap.at(soundName));
				m_soundSourceMap.erase(soundName);
			}
			else
			{
				return m_soundSourceMap.at(soundName);
			}
		}

		if (!audioDevice)
		{
			throw std::runtime_error("AudioLoader - Cannot load audio. No device has been set");
		}

		auto soundSource = audioDevice->loadSound(soundConfig.path, soundConfig.preload);

		if (!soundSource)
		{
			throw std::runtime_error("AudioLoader - Error while loading sound source from file: " + soundConfig.path);
		}

		float soundDefaultVolume = static_cast<float>(soundConfig.defaultVolume) / 100;

		if (soundDefaultVolume > 1.0f)
		{
			soundDefaultVolume = 1.0f;
		}

		soundSource->setDefaultVolume(soundDefaultVolume);

		m_soundSourceMap.insert({ soundName, soundSource });

		return soundSource;
	}
}