#pragma once

#include <GL/glew.h>

#include <string>

namespace diamond_engine
{
	struct Animation
	{
		std::string name{ "Unnamed" };
		GLuint frame{ 0 };
		GLfloat duration{ 0.0f };
		GLfloat delayBetweenFrames{ 0.0f };
		GLfloat timeBetweenFrames{ 0.0f };
	};
}
