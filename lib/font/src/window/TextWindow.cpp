#include "TextWindow.h"

namespace diamond_engine
{
	void TextWindow::setSize(const Size& size)
	{
		m_size = size;
	}

	const Size& TextWindow::getSize() const
	{
		return m_size;
	}

	void TextWindow::setCanvas(const Texture& canvas)
	{
		m_canvas = canvas;
	}

	const Texture& TextWindow::getCanvas() const
	{
		return m_canvas;
	}

	void TextWindow::setFontSize(const Size& fontSize)
	{
		m_fontSize = fontSize;
	}

	const Size& TextWindow::getFontSize() const
	{
		return m_fontSize;
	}

	void TextWindow::setCursorPos(const Size& cursorPos)
	{
		m_cursorPos = cursorPos;
	}

	const Size& TextWindow::getCursorPos() const
	{
		return m_cursorPos;
	}

	Size& TextWindow::getCursorPos()
	{
		return m_cursorPos;
	}
}