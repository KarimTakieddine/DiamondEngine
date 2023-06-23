#pragma once

#include "ComponentBuilder.h"

namespace diamond_engine {
class MeshRendererBuilder : public ComponentBuilder {
public:
	std::unique_ptr<Component> Build(const ComponentConfig* componentConfig) override;
};
}
