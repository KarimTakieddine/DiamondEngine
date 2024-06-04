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
		std::vector<std::unique_ptr<GameInstance>>& getFontInstances();
		const std::vector<std::unique_ptr<GameInstance>>& getFontInstances() const;

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
		std::vector<std::unique_ptr<GameInstance>> m_fontInstances;
		Texture m_canvas;
		Size m_fontSize;
		Size m_cursorPos;
		Size m_size;
	};
}
