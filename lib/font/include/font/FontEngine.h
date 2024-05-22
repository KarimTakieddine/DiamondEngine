#pragma once

#include <map>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include "EngineStatus.h"
#include "Font.h"
#include "Texture.h"

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
		EngineStatus setRenderDimensions(const glm::vec2& topLeft, const Texture& canvas);
		void render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		void printFont(const GLubyte symbol, GLsizei row, GLsizei column);

	private:
		std::map<GLubyte, size_t> m_symbolToIndex;
		std::vector<std::unique_ptr<GameInstance>> m_fontInstances;
		std::vector<Font> m_fonts;
	};
}
