#include <stdexcept>
#include <string_view>
#include <unordered_map>

#include "Collider2DConfig.h"
#include "Collider2DConfigParser.h"

namespace
{
	const std::unordered_map<std::string_view, diamond_engine::ColliderType> kColliderTypeConversionTable = {
		{ "character", diamond_engine::ColliderType::CHARACTER },
		{ "obstacle", diamond_engine::ColliderType::OBSTACLE }
	};
}

namespace diamond_engine
{
	/* static */ std::unique_ptr<ComponentConfig> Collider2DConfigParser::Parse(const pugi::xml_node& componentNode)
	{
		std::unique_ptr<Collider2DConfig> result = std::make_unique<Collider2DConfig>();

		pugi::xml_attribute widthAttribute = componentNode.attribute("width");
		if (widthAttribute) {
			result->setWidth(widthAttribute.as_float());
		}

		pugi::xml_attribute heightAttribute = componentNode.attribute("height");
		if (heightAttribute) {
			result->setHeight(heightAttribute.as_float());
		}

		pugi::xml_attribute typeAttribute = componentNode.attribute("type");
		if (typeAttribute)
		{
			const std::string typeAttributeString(typeAttribute.as_string("obstacle"));

			auto typeIt = kColliderTypeConversionTable.find(typeAttributeString);
			if (typeIt == kColliderTypeConversionTable.cend())
			{
				throw std::runtime_error("Failed to parse Collider2D node. Unrecognised type: " + typeAttributeString);
			}

			result->setColliderType(kColliderTypeConversionTable.at(typeAttributeString));
		}

		return result;
	}
}