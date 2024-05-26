#pragma once

#include <map>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "EngineStatus.h"
#include "Font.h"
#include "Texture.h"
#include "Size.h"

namespace diamond_engine
{
	class GameInstance;
	class RenderingSubsystem;
	class FontEngine
	{
	public:
		EngineStatus registerFont(const Font& font);
		EngineStatus allocateGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		EngineStatus releaseGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		EngineStatus setTopLeft(const glm::vec2& topLeft);
		EngineStatus setFontColor(const glm::vec3& fontColor);
		EngineStatus setFontScale(const glm::vec2& fontScale);
		void render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		void printFont(GLubyte symbol, GLsizei row, GLsizei column);
		void printString(const std::string& s);
		EngineStatus configure(const Size& fontSize, const Texture& canvas);

	private:
		std::map<GLubyte, size_t> m_symbolToIndex;
		std::vector<std::unique_ptr<GameInstance>> m_fontInstances;
		std::vector<Font> m_fonts;
		Texture m_canvas;
		Size m_fontSize;
		Size m_cursorPos;
	};
}
