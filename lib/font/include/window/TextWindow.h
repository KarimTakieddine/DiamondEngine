#pragma once

#include <vector>

#include "Texture.h"
#include "Size.h"

namespace diamond_engine
{
	class GameInstance;
	class TextWindow
	{
	public:
		void setSize(const Size& size);
		const Size& getSize() const;

		void setCanvas(const Texture& canvas);
		const Texture& getCanvas() const;

		void setFontSize(const Size& fontSize);
		const Size& getFontSize() const;

		void setCursorPos(const Size& cursorPos);
		const Size& getCursorPos() const;
		Size& getCursorPos();

	private:
		Texture m_canvas;
		Size m_fontSize;
		Size m_cursorPos;
		Size m_size;
	};
}
