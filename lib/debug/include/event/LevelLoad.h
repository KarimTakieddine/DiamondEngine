#pragma once

#include <string>

#include "DebugEvent.h"

namespace diamond_engine
{
	class LevelLoad : public DebugEvent
	{
	public:
		~LevelLoad() final override = default;

		LevelLoad(const std::string& name);

		const std::string& getName() const;
		void setName(const std::string& name);

	private:
		std::string m_name{ "Unknown" };
	};
}
