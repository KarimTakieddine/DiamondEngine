#include "ComponentBuilder.h"
#include "GameInstance.h"
#include "GameInstanceBuilder.h"
#include "GameInstanceConfig.h"

namespace diamond_engine
{
	EngineStatus buildGameInstance(const std::unique_ptr<GameInstance>& instance, const GameInstanceConfig* instanceConfig)
	{
		if (!instanceConfig)
		{
			return { "Failed to build GameInstance from null instance config", true };
		}

		EngineStatus status;
		for (const auto& renderComponentConfig : instanceConfig->getRenderConfigs())
		{
			auto renderComponent = ComponentBuilder::buildRenderComponent(renderComponentConfig.get(), &status);
			
			if (!status)
			{
				return status;
			}

			status = instance->acquireRenderComponent(std::move(renderComponent));

			if (!status)
			{
				return status;
			}
		}

		return status;
	}
}