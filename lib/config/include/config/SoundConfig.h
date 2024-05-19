#pragma once

#include <string>

namespace diamond_engine
{
	struct SoundConfig
	{
		std::string name{ "Unnamed" };
		std::string path{  };
		unsigned char defaultVolume{ 100 };
		bool preload{ false };
	};
}
