#pragma once

#include <map>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "EngineStatus.h"
#include "Texture.h"
#include "Size.h"

namespace diamond_engine
{
	class FontLibrary;
	class GameInstance;
	class RenderingSubsystem;
	class TextWindow
	{
	public:
		EngineStatus setFontLibrary(const std::shared_ptr<FontLibrary>& fontLibrary);
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
		std::shared_ptr<FontLibrary> m_fontLibrary{ nullptr };
		std::vector<std::unique_ptr<GameInstance>> m_fontInstances;
		Texture m_canvas;
		Size m_fontSize;
		Size m_cursorPos;
		Size m_size;
	};
}
