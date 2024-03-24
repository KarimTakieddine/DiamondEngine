#pragma once

#include <string>

namespace diamond_engine
{
	struct Animation
	{
		std::string name{ "Unnamed" };
		unsigned int frame{ 0 };
		float duration{ 0.0f };
		float delayBetweenFrames{ 0.0f };
		float timeBetweenFrames{ 0.0f };
	};
}
