#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "EngineStatus.h"
#include "Size.h"
#include "Texture.h"

namespace diamond_engine
{
	class GameInstance;
	class FontRenderComponent;
	class MaterialRenderComponent;

	struct LiveFont
	{
		GameInstance* instance{ nullptr };
		FontRenderComponent* renderComponent{ nullptr };
		MaterialRenderComponent* material	{ nullptr };
	};

	class FontLibrary;
	class RenderingSubsystem;
	class TextWindow;
	class FontEngine
	{
	public:
		EngineStatus setFontLibrary(const std::shared_ptr<FontLibrary>& fontLibrary);
		EngineStatus registerTextWindow(const Size& fontSize, const Texture& canvas);
		EngineStatus allocateGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		EngineStatus releaseGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem);
		EngineStatus printFont(GLubyte symbol, size_t windowIndex, GLsizei row, GLsizei column);
		EngineStatus render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem) const;
		EngineStatus printString(const std::string& s, size_t windowIndex);
		EngineStatus setWindowDimensions(size_t windowIndex, const glm::vec2& topLeft, const glm::vec2& fontScale);
		EngineStatus setWindowColor(size_t windowIndex, const glm::vec3& color);
		TextWindow* getTextWindow(size_t index) const;

	private:
		GameInstance* registerFontInstance();
		GLsizei windowToFontIndex(size_t windowIndex, GLsizei row, GLsizei column) const;

		std::vector<std::unique_ptr<TextWindow>> m_textWindows;
		std::vector<std::unique_ptr<GameInstance>> m_fontInstances;
		std::vector<LiveFont> m_liveFonts;
		std::shared_ptr<FontLibrary> m_fontLibrary{ nullptr };
	};
}
