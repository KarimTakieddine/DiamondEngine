#include <GL/glew.h>

#include <pugixml.hpp>

#include "SpriteAnimationParser.h"
#include "Collider2DComponentConfig.h"
#include "ComponentConfigParser.h"
#include "MaterialComponentConfig.h"
#include "TransformComponentConfig.h"
#include "SpriteAnimationPlayerConfig.h"
#include "Vector3Parser.h"
#include "Vector2Parser.h"

namespace
{
	const std::unordered_map<std::string, GLenum> kStringToDrawMode = {
		{ "static",		GL_STATIC_DRAW },
		{ "dynamic",	GL_DYNAMIC_DRAW }
	};

	using diamond_engine::EngineStatus;
	void setErrorStatus(EngineStatus* status, const std::string& message, bool error = true)
	{
		if (!status)
		{
			return;
		}

		*status = { message, error };
	}

	using diamond_engine::RenderComponentConfig;

	static std::unique_ptr<RenderComponentConfig> parseMaterialConfig(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		using diamond_engine::MaterialComponentConfig;
		using diamond_engine::Vector3Parser;
		using diamond_engine::Vector2Parser;

		std::unique_ptr<MaterialComponentConfig> result = std::make_unique<MaterialComponentConfig>();

		pugi::xml_attribute textureAttribute = node.attribute("texture");
		if (textureAttribute)
		{
			result->setTextureName(textureAttribute.as_string(result->getTextureName().c_str()));
		}

		pugi::xml_node colorNode = node.child("Color");
		if (colorNode)
		{
			result->setColor(Vector3Parser::Parse(colorNode));
		}

		pugi::xml_node textureOffsetNode = node.child("TextureOffset");
		if (textureOffsetNode)
		{
			result->setTextureOffset(Vector2Parser::Parse(textureOffsetNode));
		}

		return result;
	}

	static std::unique_ptr<RenderComponentConfig> parseTransformConfig(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		using diamond_engine::TransformComponentConfig;
		using diamond_engine::Vector3Parser;

		std::unique_ptr<TransformComponentConfig> result = std::make_unique<TransformComponentConfig>();
		
		pugi::xml_node positionNode = node.child("Position");
		if (positionNode)
		{
			result->setPosition(Vector3Parser::Parse(positionNode));
		}

		pugi::xml_node localScaleNode = node.child("LocalScale");
		if (localScaleNode)
		{
			result->setLocalScale(Vector3Parser::Parse(localScaleNode));
		}

		return result;
	}

	using diamond_engine::BehaviourComponentConfig;
	using diamond_engine::ColliderType;
	using diamond_engine::ColliderIgnoreFlags;

	const std::unordered_map<std::string, ColliderType> kStringToColliderType = {
		{ "obstacle",	ColliderType::OBSTACLE },
		{ "character",	ColliderType::CHARACTER },
		{ "zone",		ColliderType::ZONE }
	};

	const std::unordered_map<std::string, ColliderIgnoreFlags> kStringToIgnoreFlag = {
		{ "none",	ColliderIgnoreFlags::NONE },
		{ "up",		ColliderIgnoreFlags::UP },
		{ "down",	ColliderIgnoreFlags::DOWN },
		{ "left",	ColliderIgnoreFlags::LEFT },
		{ "right",	ColliderIgnoreFlags::RIGHT },
	};

	ColliderIgnoreFlags parseIgnoreFlags(const std::string& flagsString)
	{
		ColliderIgnoreFlags result = ColliderIgnoreFlags::NONE;

		size_t offset = 0;
		auto position = flagsString.find('|', offset);

		while (position != std::string::npos)
		{
			const std::string flagSubstring = flagsString.substr(offset, position - offset);

			const auto flagIt = kStringToIgnoreFlag.find(flagSubstring);
			if (flagIt != kStringToIgnoreFlag.cend())
			{
				result |= flagIt->second;
			}

			offset		= position + 1;
			position	= flagsString.find('|', offset);
		}

		if (offset < flagsString.size() - 1)
		{
			const std::string flagSubstring = flagsString.substr(offset, flagsString.size() - offset);
			const auto flagIt = kStringToIgnoreFlag.find(flagSubstring);
			if (flagIt != kStringToIgnoreFlag.cend())
			{
				result |= flagIt->second;
			}
		}

		return result;
	}

	static std::unique_ptr<BehaviourComponentConfig> parseCollider2DConfig(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		using diamond_engine::Collider2DComponentConfig;

		std::unique_ptr<Collider2DComponentConfig> result = std::make_unique<Collider2DComponentConfig>();

		pugi::xml_attribute widthAttribute	= node.attribute("width");
		pugi::xml_attribute heightAttribute = node.attribute("height");

		const glm::vec2& defaultSize = result->getSize();
		result->setSize({ widthAttribute.as_float(defaultSize.x), heightAttribute.as_float(defaultSize.y) });

		pugi::xml_attribute offsetXAttribute = node.attribute("xOffset");
		pugi::xml_attribute offsetYAttribute = node.attribute("yOffset");

		const glm::vec2& defaultOffset = result->getOffset();
		result->setOffset({ offsetXAttribute.as_float(defaultOffset.x), offsetYAttribute.as_float(defaultOffset.y) });

		pugi::xml_attribute typeAttribute = node.attribute("type");
		if (typeAttribute)
		{
			const std::string typeString(typeAttribute.as_string());
			auto typeIt = kStringToColliderType.find(typeString);
			if (typeIt == kStringToColliderType.cend())
			{
				setErrorStatus(outStatus, "Invalid collider type specified: " + typeString);
			}

			result->setType(typeIt->second);
		}

		pugi::xml_attribute ignoreFlagsAttribute = node.attribute("ignore");
		if (ignoreFlagsAttribute)
		{
			result->setIgnoreFlags(parseIgnoreFlags(ignoreFlagsAttribute.as_string()));
		}

		return result;
	}

	static std::unique_ptr<BehaviourComponentConfig> parseSpriteAnimationPlayerConfig(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		// TODO: Remove need for separate AnimationParser class...

		using diamond_engine::SpriteAnimationParser;
		using diamond_engine::SpriteAnimationPlayerConfig;

		std::unique_ptr<SpriteAnimationPlayerConfig> result = std::make_unique<SpriteAnimationPlayerConfig>();

		for (const auto& animationNode : node.children("Animation"))
		{
			result->addAnimation(SpriteAnimationParser::Parse(animationNode));
		}

		return result;
	}
}

namespace diamond_engine
{
	/* static */ void ComponentConfigParser::registerRenderConfig(const std::string& name, RenderParseMethod parseMethod)
	{
		renderParseMethods.insert({ name, parseMethod });
	}

	/* static */ void ComponentConfigParser::registerBehaviourConfig(const std::string& name, BehaviourParseMethod parseMethod)
	{
		behaviourParseMethods.insert({ name, parseMethod });
	}

	/* static */ std::unique_ptr<RenderComponentConfig> ComponentConfigParser::parseRenderComponent(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		const std::string nodeName(node.name());

		auto parseMethodIt = renderParseMethods.find(nodeName);
		if (parseMethodIt == renderParseMethods.cend())
		{
			setErrorStatus(outStatus, "Failed to parse render component of unknown name: " + nodeName);
			return nullptr;
		}

		return parseMethodIt->second(node, outStatus);
	}

	/* static */ std::unique_ptr<BehaviourComponentConfig> ComponentConfigParser::parseBehaviourComponent(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		const std::string nodeName(node.name());

		auto parseMethodIt = behaviourParseMethods.find(nodeName);
		if (parseMethodIt == behaviourParseMethods.cend())
		{
			setErrorStatus(outStatus, "Failed to parse behaviour component of unknown name: " + nodeName);
			return nullptr;
		}

		return parseMethodIt->second(node, outStatus);
	}

	/* static */ std::unordered_map<std::string, ComponentConfigParser::RenderParseMethod> ComponentConfigParser::renderParseMethods = {
		{ "Material",	&parseMaterialConfig	},
		{ "Transform",	&parseTransformConfig	}
	};

	/* static */ std::unordered_map<std::string, ComponentConfigParser::BehaviourParseMethod> ComponentConfigParser::behaviourParseMethods = {
		{ "Collider2D",				&parseCollider2DConfig },
		{ "SpriteAnimationPlayer",	&parseSpriteAnimationPlayerConfig }
	};
}