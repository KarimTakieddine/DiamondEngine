#pragma once

#include "InstanceConfig.h"
#include "MaterialConfig.h"
#include "MeshRenderConfig.h"
#include "TransformConfig.h"

namespace diamond_engine
{
	class SpriteInstanceConfig : public InstanceConfig
	{
	public:
		~SpriteInstanceConfig() final override = default;

		const char* getName() const final override
		{
			return "Sprite";
		}

		const MaterialConfig& getMaterialConfig() const;
		MaterialConfig& getMaterialConfig();
		void setMaterialConfig(const MaterialConfig& materialConfig);

		const MeshRenderConfig& getMeshRenderConfig() const;
		MeshRenderConfig& getMeshRenderConfig();
		void setMeshRenderConfig(const MeshRenderConfig& meshRenderConfig);

		const TransformConfig& getTransformConfig() const;
		TransformConfig& getTransformConfig();
		void setTransformConfig(const TransformConfig& transformConfig);

	private:
		MaterialConfig m_materialConfig;
		MeshRenderConfig m_meshRenderConfig;
		TransformConfig m_transformConfig;
	};
}
