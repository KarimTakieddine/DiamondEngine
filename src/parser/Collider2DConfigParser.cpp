#include "Collider2DConfig.h"
#include "Collider2DConfigParser.h"

namespace diamond_engine
{
	/* static */ std::unique_ptr<ComponentConfig> Collider2DConfigParser::Parse(const pugi::xml_node& componentNode)
	{
		std::unique_ptr<Collider2DConfig> result = std::make_unique<Collider2DConfig>();

		pugi::xml_attribute widthAttribute = componentNode.attribute("width");
		if (widthAttribute) {
			result->setWidth(widthAttribute.as_uint());
		}

		pugi::xml_attribute heightAttribute = componentNode.attribute("height");
		if (heightAttribute) {
			result->setHeight(heightAttribute.as_uint());
		}

		return result;
	}
}