#include "ComponentBuilder.h"
#include "MaterialComponentConfig.h"
#include "MaterialRenderComponent.h"
#include "SharedMeshStore.h"
#include "TextureLoader.h"
#include "TransformComponentConfig.h"
#include "TransformRenderComponent.h"

namespace
{
	using diamond_engine::EngineStatus;
	using diamond_engine::IRenderComponent;
	using diamond_engine::RenderComponentConfig;

	static std::unique_ptr<IRenderComponent> buildMaterialComponent(const RenderComponentConfig* config, EngineStatus* outStatus /* = nullptr */)
	{
		using diamond_engine::MaterialComponentConfig;
		using diamond_engine::MaterialRenderComponent;
		using diamond_engine::TextureLoader;

		const MaterialComponentConfig* materialComponentConfig = dynamic_cast<const MaterialComponentConfig*>(config);

		if (!materialComponentConfig)
		{
			if (outStatus)
			{
				*outStatus = { "Failed to cast supplied config to MaterialComponentConfig", true };
			}

			return nullptr;
		}

		std::unique_ptr<MaterialRenderComponent> result = std::make_unique<MaterialRenderComponent>();

		result->setTexture(TextureLoader::getInstance()->GetTexture(materialComponentConfig->getTextureName()));
		result->setColor(materialComponentConfig->getColor());
		result->setTextureOffset(materialComponentConfig->getTextureOffset());

		return result;
	}

	static std::unique_ptr<IRenderComponent> buildTransformComponent(const RenderComponentConfig* config, EngineStatus* outStatus /* = nullptr */)
	{
		using diamond_engine::TransformComponentConfig;
		using diamond_engine::TransformRenderComponent;

		const TransformComponentConfig* transformConfig = dynamic_cast<const TransformComponentConfig*>(config);

		if (!transformConfig)
		{
			if (outStatus)
			{
				*outStatus = { "Failed to cast supplied config to TransformComponentConfig", true };
			}

			return nullptr;
		}

		std::unique_ptr<TransformRenderComponent> result = std::make_unique<TransformRenderComponent>();

		result->setPosition(transformConfig->getPosition());

		return result;
	}
}

namespace diamond_engine
{
	/* static */ void ComponentBuilder::registerRenderComponent(const std::string& name, RenderBuildMethod buildMethod)
	{
		renderBuildMethods.insert({ name, buildMethod });
	}

	/* static */ std::unique_ptr<IRenderComponent> ComponentBuilder::buildRenderComponent(const RenderComponentConfig* config, EngineStatus* outStatus /* = nullptr */)
	{
		if (!config)
		{
			if (outStatus)
			{
				*outStatus = { "Failed to build RenderComponent from null component config", true };
			}

			return nullptr;
		}

		const std::string componentName(config->getName());

		auto buildMethodIt = renderBuildMethods.find(componentName);
		if (buildMethodIt == renderBuildMethods.cend())
		{
			if (outStatus)
			{
				*outStatus = { "Failed to build RenderComponent of unknown name: " + componentName, true };
			}

			return nullptr;
		}

		return buildMethodIt->second(config, outStatus);
	}

	/* static */ std::unordered_map<std::string, ComponentBuilder::RenderBuildMethod> ComponentBuilder::renderBuildMethods =
	{
		{ "Material",	::buildMaterialComponent },
		{ "Transform",	::buildTransformComponent }
	};
}