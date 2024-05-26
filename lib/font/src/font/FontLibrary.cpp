#include "FontLibrary.h"

namespace diamond_engine
{
	void FontLibrary::registerFont(GLubyte symbol, const Font& font)
	{
		m_fonts.insert({ symbol, font });
	}

	void FontLibrary::unregisterFont(GLubyte symbol)
	{
		auto it = m_fonts.find(symbol);

		if (it == m_fonts.cend())
			return;

		m_fonts.erase(it);
	}

	Font FontLibrary::getFont(GLubyte symbol) const
	{
		auto it = m_fonts.find(symbol);

		return it == m_fonts.cend() ? Font{ -1, -1 } : it->second;
	}
}
