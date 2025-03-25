#include "EngineStorage.h"

namespace diamond_engine
{
	bool EngineStorage::getVariant(const std::string& key, EngineVariant* variant) const
	{
		if (!variant)
		{
			return false;
		}

		const auto variantIt = m_data.find(key);
		if (variantIt == m_data.cend())
		{
			return false;
		}

		*variant = variantIt->second;

		return true;
	}

	void EngineStorage::setVariant(const std::string& key, GLint value)
	{
		auto variantIt = m_data.find(key);

		if (variantIt == m_data.cend())
		{
			m_data.emplace(key, value);
		}
		else
		{
			variantIt->second = value;
		}
	}

	void EngineStorage::setVariant(const std::string& key, GLuint value)
	{
		auto variantIt = m_data.find(key);

		if (variantIt == m_data.cend())
		{
			m_data.emplace(key, value);
		}
		else
		{
			variantIt->second = value;
		}
	}

	void EngineStorage::setVariant(const std::string& key, GLfloat value)
	{
		auto variantIt = m_data.find(key);

		if (variantIt == m_data.cend())
		{
			m_data.emplace(key, value);
		}
		else
		{
			variantIt->second = value;
		}
	}

	void EngineStorage::setVariant(const std::string& key, GLdouble value)
	{
		auto variantIt = m_data.find(key);

		if (variantIt == m_data.cend())
		{
			m_data.emplace(key, value);
		}
		else
		{
			variantIt->second = value;
		}
	}

	void EngineStorage::setVariant(const std::string& key, const std::string& value)
	{
		auto variantIt = m_data.find(key);

		if (variantIt == m_data.cend())
		{
			m_data.emplace(key, value);
		}
		else
		{
			variantIt->second = value;
		}
	}
}