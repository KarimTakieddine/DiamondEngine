#include <GL/glew.h>

#include <pugixml.hpp>

#include "Collider2DComponentConfig.h"
#include "ComponentConfigParser.h"
#include "MaterialComponentConfig.h"
#include "TransformComponentConfig.h"
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

		return result;
	}

	using diamond_engine::BehaviourComponentConfig;
	using diamond_engine::ColliderType;

	const std::unordered_map<std::string, ColliderType> kStringToColliderType = {
		{ "obstacle", ColliderType::OBSTACLE },
		{ "character", ColliderType::CHARACTER }
	};

	static std::unique_ptr<BehaviourComponentConfig> parseCollider2DConfig(const pugi::xml_node& node, EngineStatus* outStatus)
	{
		using diamond_engine::Collider2DComponentConfig;
		using diamond_engine::Size;

		std::unique_ptr<Collider2DComponentConfig> result = std::make_unique<Collider2DComponentConfig>();

		pugi::xml_attribute widthAttribute	= node.attribute("width");
		pugi::xml_attribute heightAttribute = node.attribute("height");

		const Size& defaultSize = result->getSize();
		result->setSize({ widthAttribute.as_int(defaultSize.width), heightAttribute.as_int(defaultSize.height) });

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
		{ "Collider2D", &parseCollider2DConfig }
	};
}