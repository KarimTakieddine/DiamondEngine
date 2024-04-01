#include <pugixml.hpp>

#include "EngineStatus.h"
#include "GameInstanceConfigParser.h"
#include "SpriteInstanceConfig.h"
#include "Vector2Parser.h"
#include "Vector3Parser.h"

namespace
{
	using diamond_engine::GameInstanceType;
	using diamond_engine::MeshType;

	const std::unordered_map<std::string, GameInstanceType> kStringToInstanceType =
	{
		{ "sprite", GameInstanceType::SPRITE }
	};

	const std::unordered_map<std::string, MeshType> kStringToMeshType =
	{
		{ "quad", MeshType::QUAD }
	};

	const std::unordered_map<std::string, GLenum> kStringToDrawMode =
	{
		{ "static",		GL_STATIC_DRAW },
		{ "dynamic",	GL_DYNAMIC_DRAW }
	};
}

namespace diamond_engine
{
	/* static */ std::unique_ptr<GameInstanceConfig> GameInstanceConfigParser::parse(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		pugi::xml_attribute typeAttribute = node.attribute("type");
		if (!typeAttribute)
		{
			if (outStatus)
				*outStatus = { "Failed to parse GameInstanceConfig. No instance \"type\" attribute was set", true };

			return nullptr;
		}

		auto typeIt = ::kStringToInstanceType.find(typeAttribute.as_string());
		if (typeIt == ::kStringToInstanceType.cend())
		{
			if (outStatus)
				*outStatus = { "Failed to parse GameInstanceConfig. Invalid type found", true };

			return nullptr;
		}

		if (outStatus)
			*outStatus = { };

		switch (typeIt->second)
		{
			case GameInstanceType::SPRITE: {
				std::unique_ptr<SpriteInstanceConfig> result = std::make_unique<SpriteInstanceConfig>();
				result->setMaterialConfig(parseMaterialConfig(node, outStatus));
				result->setMeshRenderConfig(parseMeshRenderConfig(node, outStatus));
				// TODO: Transform config

				return result;
			}
			default: {
				if (outStatus)
					*outStatus = { "Failed to parse GameInstanceConfig. Unknown or unsupported type found", true };

				return nullptr;
			}
		}
	}

	/* static */ MaterialConfig GameInstanceConfigParser::parseMaterialConfig(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		MaterialConfig result;

		pugi::xml_node materialNode = node.child("Material");

		if (!materialNode)
		{
			if (outStatus)
				*outStatus = { "Failed to parse MaterialConfig. No root <Material/> node was found. Returning default", true };

			return result;
		}

		pugi::xml_attribute textureAttribute = materialNode.attribute("texture");
		if (textureAttribute)
		{
			result.SetTextureName(textureAttribute.as_string(result.GetTextureName().c_str()));
		}

		pugi::xml_node colorNode = materialNode.child("Color");
		if (colorNode)
		{
			result.SetColor(Vector3Parser::Parse(colorNode));
		}

		pugi::xml_node textureOffsetNode = materialNode.child("TextureOffset");
		if (textureOffsetNode)
		{
			result.SetTextureOffset(Vector2Parser::Parse(textureOffsetNode));
		}

		if (outStatus)
			*outStatus = { };

		return result;
	}

	/* static */ MeshRenderConfig GameInstanceConfigParser::parseMeshRenderConfig(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		MeshRenderConfig result;

		pugi::xml_node meshRenderNode = node.child("MeshRender");

		if (!meshRenderNode)
		{
			if (outStatus)
				*outStatus = { "Failed to parse MeshRenderConfig. No root <MeshRender/> node was found. Returning default", true };

			return result;
		}

		pugi::xml_attribute typeAttribute = meshRenderNode.attribute("type");

		if (!typeAttribute)
		{
			if (outStatus)
				*outStatus = { "Failed to parse MeshRenderConfig. No \"type\" attribute was set", true };

			return result;
		}

		auto meshTypeIt = kStringToMeshType.find(typeAttribute.as_string());
		if (meshTypeIt == kStringToMeshType.cend())
		{
			if (outStatus)
				*outStatus = { "Failed to parse MeshRenderConfig. Invalid or unknown mesh type found", true };

			return result;
		}

		result.SetMeshType(meshTypeIt->second);

		pugi::xml_attribute drawModeAttribute = meshRenderNode.attribute("drawMode");
		if (drawModeAttribute)
		{
			auto drawModeIt = kStringToDrawMode.find(drawModeAttribute.as_string());
			if (drawModeIt == kStringToDrawMode.cend())
			{
				if (outStatus)
					*outStatus = { "Failed to parse MeshRenderConfig. Invalid or unknown draw mode found", true };
			}
		}

		if (outStatus)
			*outStatus = { };

		return result;
	}
}