#include "GameInstance.h"
#include "FontEngine.h"
#include "FontLibrary.h"
#include "FontRenderComponent.h"
#include "MaterialRenderComponent.h"
#include "RenderingSubsystem.h"
#include "TextWindow.h"
#include "Vertex.h"

namespace diamond_engine
{
	EngineStatus FontEngine::setFontLibrary(const std::shared_ptr<FontLibrary>& fontLibrary)
	{
		if (!fontLibrary)
			return { "Attempt to link TextWindow to NULL font library", true };

		m_fontLibrary = fontLibrary;

		return { };
	}

	EngineStatus FontEngine::registerTextWindow(const Size& fontSize, const Texture& canvas)
	{
		std::unique_ptr<TextWindow> textWindow = std::make_unique<TextWindow>();

		auto& fontInstances = textWindow->getFontInstances();

		const GLsizei rows		= canvas.height / fontSize.height;
		const GLsizei columns	= canvas.width / fontSize.width;
		const GLsizei count		= rows * columns;

		fontInstances.reserve(count);
		for (size_t i = 0; i < count; ++i)
		{
			if (!registerFontInstance())
				return { "FontEngine failed to register font instance", true };
		}

		textWindow->setCanvas(canvas);
		textWindow->setFontSize(fontSize);
		textWindow->setSize({ columns, rows });

		m_textWindows.push_back(std::move(textWindow));

		return { };
	}

	EngineStatus FontEngine::allocateGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return { "FontEngine failed to allocate graphics memory. No rendering subsystem assigned", true };

		renderingSubsystem->allocateVertexState(
			"font_renderer",
			GL_STATIC_DRAW,
			{
				VertexAttribute{ "position", 0, 2, sizeof(Vertex), GL_FLOAT },
				VertexAttribute{ "uv", 2 * sizeof(glm::vec3), 2, sizeof(Vertex), GL_FLOAT }
			});

		for (size_t windowIndex = 0; windowIndex < m_textWindows.size(); ++windowIndex)
		{
			const auto& textWindow = m_textWindows[windowIndex];

			for (GLsizei row = 0; row < textWindow->getSize().height; ++row)
			{
				for (GLsizei column = 0; column < textWindow->getSize().width; ++column)
				{
					const GLsizei i = windowToFontIndex(windowIndex, row, column);

					const auto& fontInstance = m_fontInstances[i];
					renderingSubsystem->registerRenderObject("font_renderer", fontInstance->getRenderComponents());
					fontInstance->getRenderComponent<MaterialRenderComponent>("Material")->setTexture(textWindow->getCanvas());
				}
			}
		}

		return { };
	}

	EngineStatus FontEngine::releaseGraphicsMemory(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem)
	{
		if (!renderingSubsystem)
			return { "FontEngine failed to release graphics memory. No rendering subsystem assigned", true };

		for (GLsizei windowIndex = m_textWindows.size() - 1; windowIndex >= 0; --windowIndex)
		{
			const auto& textWindow = m_textWindows[windowIndex];

			for (GLsizei row = textWindow->getSize().height - 1; row >= 0; --row)
			{
				for (GLsizei column = textWindow->getSize().width - 1; column >= 0; --column)
				{
					const auto& fontInstance = m_fontInstances[windowToFontIndex(windowIndex, row, column)];
					renderingSubsystem->unregisterRenderObject("font_renderer", fontInstance->getRenderComponents());
				}
			}
		}

		renderingSubsystem->releaseVertexState();

		return { };
	}

	GameInstance* FontEngine::registerFontInstance()
	{
		std::unique_ptr<GameInstance> instance = std::make_unique<GameInstance>();

		LiveFont liveFont;

		std::unique_ptr<FontRenderComponent> renderComponent	= std::make_unique<FontRenderComponent>();
		liveFont.renderComponent								= renderComponent.get();
		instance->acquireRenderComponent(std::move(renderComponent));

		std::unique_ptr<MaterialRenderComponent> material	= std::make_unique<MaterialRenderComponent>();
		liveFont.material									= material.get();
		instance->acquireRenderComponent(std::move(material));

		auto result		= instance.get();
		liveFont.instance					= result;
		m_fontInstances.push_back(std::move(instance));

		m_liveFonts.push_back(liveFont);

		return result;
	}

	EngineStatus FontEngine::printFont(GLubyte symbol, size_t windowIndex, GLsizei row, GLsizei column)
	{
		const GLsizei flattenedIndex = windowToFontIndex(windowIndex, row, column);

		if (flattenedIndex == -1 || flattenedIndex >= m_liveFonts.size())
			return {
				"Cannot print font: " + std::to_string(symbol) + " to out of bounds instance index: " + std::to_string(flattenedIndex),
				true
			};

		const Font font = m_fontLibrary->getFont(symbol);

		if (font.row == -1 || font.column == -1)
			return {
				"Cannot print font: " + std::to_string(symbol) + ". Invalid font data",
				true
			};

		const auto& liveFont = m_liveFonts[flattenedIndex];

		const glm::vec2& fontSize = liveFont.renderComponent->getFontSize();

		liveFont.renderComponent->setVXOffset({ column * fontSize.x, -row * fontSize.y });
		liveFont.material->setTextureOffset({ fontSize.x * font.column, fontSize.y * font.row });
		liveFont.instance->setActive(true);

		return { };
	}

	EngineStatus FontEngine::render(const std::unique_ptr<RenderingSubsystem>& renderingSubsystem) const
	{
		if (!renderingSubsystem)
			return { "Font engine cannot render. No rendering subsystem was assigned", true };

		renderingSubsystem->render("font_renderer", m_fontInstances);

		return { };
	}

	GLsizei FontEngine::windowToFontIndex(size_t windowIndex, GLsizei row, GLsizei column) const
	{
		if (windowIndex >= m_textWindows.size())
			return -1;

		const auto& textWindow = m_textWindows[windowIndex];

		const Size& windowSize	= textWindow->getSize();
		const GLsizei width		= windowSize.width;

		GLsizei offset = 0;
		if (windowIndex > 0)
		{
			const auto& previousWindow = m_textWindows[windowIndex - 1];

			const Size& previousWindowSize	= textWindow->getSize();
			offset							= previousWindowSize.width * previousWindowSize.height;
		}

		return ( width * row ) + column + offset;
	}

	EngineStatus FontEngine::printString(const std::string& s, size_t windowIndex)
	{
		if (windowIndex >= m_textWindows.size())
			return { "Cannot print string: " + s + ". Out of bounds TextWindow access: " + std::to_string(windowIndex), true };

		const auto& textWindow	= m_textWindows[windowIndex];
		auto& cursorPos				= textWindow->getCursorPos();

		const GLsizei size = static_cast<GLsizei>(s.size());

		if (size == 0)
		{
			for (const auto& fontInstance : m_textWindows[windowIndex]->getFontInstances())
			{
				fontInstance->setActive(false);
			}

			cursorPos = { 0, 0 };
		}

		GLsizei& cursorRow		= cursorPos.height;
		GLsizei& cursorColumn	= cursorPos.width;

		const GLsizei rows		= textWindow->getSize().height;
		const GLsizei columns	= textWindow->getSize().width;

		for (char c : s)
		{
			const GLubyte symbol = static_cast<GLubyte>(c);

			if (symbol == '\n')
			{
				if (cursorRow >= rows)
					cursorRow = 0;

				for (GLsizei i = cursorColumn; i < columns; ++i)
				{
					m_fontInstances[windowToFontIndex(windowIndex, cursorRow, i)]->setActive(false);
				}

				++cursorRow;

				cursorColumn = 0;

				continue;
			}

			if (cursorColumn >= columns)
			{
				++cursorRow;

				cursorColumn = 0;
			}

			if (cursorRow >= rows)
			{
				cursorRow		= 0;
				cursorColumn	= 0;
			}

			const EngineStatus printStatus = printFont(symbol, windowIndex, cursorRow, cursorColumn);

			if (!printStatus)
				return printStatus;

			++cursorColumn;
		}

		return { };
	}

	EngineStatus FontEngine::setWindowDimensions(size_t windowIndex, const glm::vec2& topLeft, const glm::vec2& fontScale)
	{
		if (windowIndex >= m_textWindows.size())
			return { "FontEngine cannot set dimensions of TextWindow at invalid index: " + std::to_string(windowIndex), true};

		const GLsizei offset = windowToFontIndex(windowIndex, 0, 0);
		const auto& textWindow = m_textWindows[windowIndex];

		const Size& fontSize	= textWindow->getFontSize();
		const Texture& canvas	= textWindow->getCanvas();

		const GLfloat strideX = static_cast<GLfloat>(fontSize.width) / canvas.width;
		const GLfloat strideY = static_cast<GLfloat>(fontSize.height) / canvas.height;

		for (GLsizei i = 0; i < textWindow->getSize().width * textWindow->getSize().height; ++i)
		{
			const size_t fontInstanceIndex = static_cast<size_t>(offset + i);

			auto* renderComponent = m_fontInstances[fontInstanceIndex]->getRenderComponent<FontRenderComponent>("FontRenderComponent");
			renderComponent->setCTopLeft(topLeft + glm::vec2{ -strideX * (1.0f - fontScale.x), strideY * (1.0f - fontScale.y) });
			renderComponent->setFontScale(fontScale);
			renderComponent->setFontSize({ strideX, strideY });
		}

		return { };
	}

	EngineStatus FontEngine::setWindowColor(size_t windowIndex, const glm::vec3& color)
	{
		if (windowIndex >= m_textWindows.size())
			return { "FontEngine cannot set color of TextWindow at invalid index: " + std::to_string(windowIndex), true };

		const GLsizei offset = windowToFontIndex(windowIndex, 0, 0);
		const auto& textWindow = m_textWindows[windowIndex];

		for (GLsizei i = 0; i < textWindow->getSize().width * textWindow->getSize().height; ++i)
		{
			const size_t fontInstanceIndex = static_cast<size_t>(offset + i);

			m_fontInstances[fontInstanceIndex]->getRenderComponent<MaterialRenderComponent>("Material")->setColor(color);
		}

		return { };
	}

	TextWindow* FontEngine::getTextWindow(size_t index) const
	{
		if (index >= m_textWindows.size())
			return nullptr;

		return m_textWindows[index].get();
	}
}