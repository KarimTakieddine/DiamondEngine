#pragma once

#include <unordered_map>

#include <GL/glew.h>

#include "Font.h"

namespace diamond_engine
{
	class FontLibrary
	{
	public:
		void registerFont(GLubyte symbol, const Font& font);
		void unregisterFont(GLubyte symbol);

		Font getFont(GLubyte symbol) const;

	private:
		std::unordered_map<GLubyte, Font> m_fonts;
	};
}
