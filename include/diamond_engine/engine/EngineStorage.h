#pragma once

#include <GL/glew.h>

#include <unordered_map>

#include "EngineVariant.h"

namespace diamond_engine
{
	class EngineStorage
	{
	public:
		bool getVariant(const std::string& key, EngineVariant* variant) const;

		void setVariant(const std::string& key, GLint value);
		void setVariant(const std::string& key, GLuint value);
		void setVariant(const std::string& key, GLfloat value);
		void setVariant(const std::string& key, GLdouble value);
		void setVariant(const std::string& key, const std::string& value);

	private:
		std::unordered_map<std::string, EngineVariant> m_data;
	};
}
