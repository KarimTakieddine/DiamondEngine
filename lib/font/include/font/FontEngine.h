#pragma once

#include <map>
#include <memory>
#include <vector>

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
		EngineStatus allocateGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem, const Texture& canvas);
		EngineStatus releaseGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		void render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		void printFont(const GLubyte symbol, GLsizei row, GLsizei column);

	private:
		std::map<GLubyte, size_t> m_symbolToIndex;
		std::vector<std::unique_ptr<GameInstance>> m_fontInstances;
		std::vector<Font> m_fonts;
	};
}
