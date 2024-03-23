#include <filesystem>
#include <stdexcept>

#include <irrKlang.h>

#include "AudioDevice.h"
#include "AudioLoader.h"
#include "AudioMetadataParser.h"

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

		auto soundMetadataList = AudioMetadataParser::parse(metadataPath.string());

		m_soundSourceMap.clear();
		for (auto& soundMetadata : soundMetadataList)
		{
			std::filesystem::path soundPath(soundMetadata.path);

			if (!soundPath.is_absolute())
			{
				soundPath = rootDirectoryPath / soundPath;
				
				soundPath.make_preferred();

				soundMetadata.path = soundPath.string();
			}

			if (!std::filesystem::exists(soundPath) || !std::filesystem::is_regular_file(soundPath))
			{
				throw std::runtime_error("AudioLoader - Cannot find referenced sound file: " + soundPath.string());
			}

			loadSound(soundMetadata, audioDevice);
		}
	}

	irrklang::ISoundSource* AudioLoader::loadSound(const SoundMetadata& soundMetadata, AudioDevice* audioDevice, bool forceReload /* = false */)
	{
		const std::string& soundName = soundMetadata.name;

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

		auto soundSource = audioDevice->loadSound(soundMetadata.path, soundMetadata.preload);

		if (!soundSource)
		{
			throw std::runtime_error("AudioLoader - Error while loading sound source from file: " + soundMetadata.path);
		}

		float soundDefaultVolume = static_cast<float>(soundMetadata.defaultVolume) / 100;

		if (soundDefaultVolume > 1.0f)
		{
			soundDefaultVolume = 1.0f;
		}

		soundSource->setDefaultVolume(soundDefaultVolume);

		m_soundSourceMap.insert({ soundName, soundSource });

		return soundSource;
	}
}