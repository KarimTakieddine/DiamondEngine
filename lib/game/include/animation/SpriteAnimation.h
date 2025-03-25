#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

#include "SpriteAnimationFrame.h"

namespace diamond_engine
{
	struct SpriteAnimation
	{
		std::vector<SpriteAnimationFrame> frames;
		std::string name			{ "Unnamed" };
		GLfloat durationS			{ 0.0f };
		GLfloat interFrameDelayS	{ 0.0f };
		GLfloat timeBetweenFrames	{ 0.0f };
		GLuint currentFrame			{ 0 };
	};
}
