#include "SpriteInstanceConfig.h"

namespace diamond_engine
{
	const MaterialConfig& SpriteInstanceConfig::getMaterialConfig() const
	{
		return m_materialConfig;
	}

	MaterialConfig& SpriteInstanceConfig::getMaterialConfig()
	{
		return m_materialConfig;
	}

	void SpriteInstanceConfig::setMaterialConfig(const MaterialConfig& materialConfig)
	{
		m_materialConfig = materialConfig;
	}

	const MeshRenderConfig& SpriteInstanceConfig::getMeshRenderConfig() const
	{
		return m_meshRenderConfig;
	}

	MeshRenderConfig& SpriteInstanceConfig::getMeshRenderConfig()
	{
		return m_meshRenderConfig;
	}

	void SpriteInstanceConfig::setMeshRenderConfig(const MeshRenderConfig& meshRenderConfig)
	{
		m_meshRenderConfig = meshRenderConfig;
	}

	const TransformConfig& SpriteInstanceConfig::getTransformConfig() const
	{
		return m_transformConfig;
	}

	TransformConfig& SpriteInstanceConfig::getTransformConfig()
	{
		return m_transformConfig;
	}

	void SpriteInstanceConfig::setTransformConfig(const TransformConfig& transformConfig)
	{
		m_transformConfig = transformConfig;
	}
}