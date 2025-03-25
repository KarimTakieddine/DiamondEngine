#include "LevelLoad.h"

namespace diamond_engine
{
	LevelLoad::LevelLoad(const std::string& name) : m_name(name) { }

	const std::string& LevelLoad::getName() const
	{
		return m_name;
	}

	void LevelLoad::setName(const std::string& name)
	{
		m_name = name;
	}
}