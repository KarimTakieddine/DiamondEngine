#include "Collider2D.h"
#include "Collider2DConfig.h"
#include "Collider2DBuilder.h"

namespace diamond_engine
{
	/* static */ std::unique_ptr<Component> Colllider2DBuilder::Build(const std::shared_ptr<GLAllocator>& bufferAllocator, const ComponentConfig* componentConfig)
	{
		Collider2DConfig const* collider2DConfig = dynamic_cast<Collider2DConfig const*>(componentConfig);

		std::unique_ptr<Collider2D> result = std::make_unique<Collider2D>();

		result->setColliderType(collider2DConfig->getColliderType());
		result->setWidth(collider2DConfig->getWidth());
		result->setHeight(collider2DConfig->getHeight());

		return result;
	}
}