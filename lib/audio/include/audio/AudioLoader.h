#pragma once

#include <string>
#include <unordered_map>

#include "SoundConfig.h"

namespace irrklang { class ISoundSource; }

namespace diamond_engine
{
	class AudioDevice;
	class AudioLoader
	{
	public:
		static const std::string kAudioMetadataFilename;

		void load(const std::string& rootDirectory, AudioDevice* audioDevice);
		irrklang::ISoundSource* loadSound(const SoundConfig& soundConfig, AudioDevice* audioDevice, bool forceReload = false);

	private:
		std::unordered_map<std::string, irrklang::ISoundSource*> m_soundSourceMap;
	};
}
