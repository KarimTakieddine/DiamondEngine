#pragma once

#include <string>

namespace irrklang { class ISoundEngine; class ISoundSource; }

namespace diamond_engine
{
	class AudioDevice
	{
	public:
		AudioDevice();
		~AudioDevice();

		irrklang::ISoundSource* loadSound(const std::string& file, bool preload = false);

		void unloadSound(irrklang::ISoundSource* source);

		void playSound2D(irrklang::ISoundSource* source, bool loop = false);

	private:
		irrklang::ISoundEngine* m_soundEngine{ nullptr };
	};
}
