#include <filesystem>
#include <stdexcept>

#include "SpriteSheetLoader.h"
#include "SpriteSheetMetadataParser.h"

namespace diamond_engine
{
	/* static */ const std::string SpriteSheetLoader::kMetadataFilename = "spriteSheetMetadata.xml";

	void SpriteSheetLoader::setSharedTextureLoader(const std::shared_ptr<TextureLoader>& sharedTextureLoader)
	{
		m_sharedTextureLoader = sharedTextureLoader;
	}

	const SpriteSheet& SpriteSheetLoader::getSpriteSheet(const std::string& name) const
	{
		auto it = m_spriteSheets.find(name);

		if (it == m_spriteSheets.cend())
		{
			throw std::runtime_error("Could not find sprite sheet named: " + name);
		}

		return it->second;
	}

	void SpriteSheetLoader::loadSpriteSheets(const SpriteSheetList& spriteSheets)
	{
		if (!m_sharedTextureLoader)
		{
			throw std::runtime_error("No shared TextureLoader instance has been assigned to SpriteSheetLoader");
		}

		m_spriteSheets.clear();

		for (const auto& spriteSheetData : spriteSheets)
		{
			const std::string spriteSheetName	= spriteSheetData.getName();
			const std::string textureName		= spriteSheetData.getSpriteSheet();

			if (m_spriteSheets.find(spriteSheetName) != m_spriteSheets.cend())
			{
				throw std::runtime_error("Cannot load two or more sprite sheets with the same name: " + spriteSheetName);
			}

			const Texture texture = m_sharedTextureLoader->GetTexture(textureName);

			if (texture.index == 0)
			{
				throw std::runtime_error("Failed to load sprite sheet. Invalid texture: " + textureName);
			}

			const size_t textureSize = texture.width * texture.height * 4 * sizeof(GLubyte);

			glBindTexture(GL_TEXTURE_2D, texture.index);
			GLubyte* textureData = new GLubyte[textureSize]();

			/*
			* If the selected texture image does not contain four components, the following mappings are applied:
			* Single-component textures are treated as RGBA buffers with red set to the single-component value, green set to 0, blue set to 0, and alpha set to 1.
			* Two-component textures are treated as RGBA buffers with red set to the value of component zero, alpha set to the value of component one, and green and blue set to 0.
			* Finally, three-component textures are treated as RGBA buffers with red set to component zero, green set to component one, blue set to component two, and alpha set to 1.
			*/

			glGetTexImage(GL_TEXTURE_2D, 0, texture.format, GL_UNSIGNED_BYTE, textureData);

			SpriteSheet spriteSheet;
			std::vector<Texture>& frames = spriteSheet.frames;
			
			for (const auto& frameData : spriteSheetData.getFrames())
			{
				const GLuint frameX			= frameData.x;
				const GLuint frameY			= frameData.y;
				const GLuint frameWidth		= frameData.width;
				const GLuint frameHeight	= frameData.height;

				Texture frameTexture	= texture;
				frameTexture.format		= GL_RGBA;
				frameTexture.width		= frameWidth;
				frameTexture.height		= frameHeight;

				const size_t frameSize	= frameWidth * frameHeight * 4 * sizeof(GLubyte);
				GLubyte* frameData		= new GLubyte[frameSize]();
				size_t frameDataIndex	= 0;

				for (GLuint y = frameY; y < frameY + frameHeight; ++y)
				{
					for (GLuint x = frameX; x < frameX + frameWidth; ++x)
					{
						const size_t dataIndex	= (y * texture.width + x) * 4 * sizeof(GLubyte);
						const size_t redIndex	= dataIndex;
						const size_t greenIndex = dataIndex + 1;
						const size_t blueIndex	= dataIndex + 2;
						const size_t alphaIndex = dataIndex + 3;

						if (dataIndex > textureSize - 4)
						{
							throw std::runtime_error("Out-of-bounds texture access by sprite sheet. X: " + std::to_string(x) + ". Y: " + std::to_string(y) + ". Index: " + std::to_string(dataIndex));
						}

						const size_t frameDataIndex = ((y - frameY) * frameWidth + (x - frameX)) * 4 * sizeof(GLubyte);

						if (frameDataIndex > frameSize - 4)
						{
							throw std::runtime_error("Out-of-bounds frame access by sprite sheet. X: " + std::to_string(x) + ". Y: " + std::to_string(y) + ". Index: " + std::to_string(frameDataIndex));
						}

						frameData[frameDataIndex]		= textureData[redIndex];
						frameData[frameDataIndex + 1]	= textureData[greenIndex];
						frameData[frameDataIndex + 2]	= textureData[blueIndex];
						frameData[frameDataIndex + 3]	= textureData[alphaIndex];
					}
				}

				m_sharedTextureLoader->allocateTexture(frameTexture, frameData);
				frames.push_back(frameTexture);

				delete[] frameData;
			}

			m_spriteSheets.insert({ spriteSheetName, spriteSheet });

			delete[] textureData;
		}
	}

	void SpriteSheetLoader::load(const std::string& directory)
	{
		std::filesystem::path metadataPath(directory);
		
		if (!std::filesystem::is_directory(metadataPath))
		{
			throw std::runtime_error("Sprite sheet path: " + directory + " is not a directory");
		}
		
		metadataPath /= kMetadataFilename;
		metadataPath.make_preferred();

		if (!std::filesystem::exists(metadataPath) || !std::filesystem::is_regular_file(metadataPath))
		{
			throw std::runtime_error("Could not find: " + kMetadataFilename + " sprite sheet metadata file in: " + directory);
		}

		loadSpriteSheets(SpriteSheetMetadataParser::Parse(metadataPath.string()));
	}
}