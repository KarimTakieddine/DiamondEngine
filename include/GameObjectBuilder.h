#pragma once

#include <memory>

namespace diamond_engine {
class GameObject;

class GameObjectBuilder {
public:
	void Reset();

	virtual void AddComponents() = 0;

	std::unique_ptr<GameObject> Finalize();

protected:
	std::unique_ptr<GameObject> m_gameObject{ nullptr };
};
}
