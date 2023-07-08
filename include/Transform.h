#pragma once

#include <string>
#include <type_traits>

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace diamond_engine {
	enum class DirtyFlag : GLubyte {
		UNSET	= 0,
		SET		= 1
	};

	inline DirtyFlag operator|(DirtyFlag l, GLubyte r) {
		return static_cast<DirtyFlag>(
			static_cast<std::underlying_type_t<DirtyFlag>>(l) | r);
	}

	inline DirtyFlag operator|=(DirtyFlag& l, DirtyFlag r) {
		l = l | static_cast<std::underlying_type_t<DirtyFlag>>(r);
		return l;
	}

	inline DirtyFlag operator&(DirtyFlag l, DirtyFlag r) {
		return static_cast<DirtyFlag>(
			static_cast<std::underlying_type_t<DirtyFlag>>(l) &
			static_cast<std::underlying_type_t<DirtyFlag>>(r));
	}

class Transform {
public:
	static const std::string kModelUniformLocation;

	void Translate(const glm::vec3& displacement);

	void Rotate(GLfloat degrees, const glm::vec3& axis);

	void SetLocalRotation(GLfloat degrees, const glm::vec3& axis);

	void SetLocalScale(const glm::vec3& scale);

	const glm::mat4& GetLocalToWorldMatrix() const {
		return m_localToWorld;
	}

	const glm::mat4& GetLocalRotationMatrix() const {
		return m_localRotation;
	}

	const glm::mat4& GetLocalScaleMatrix() const {
		return m_localScale;
	}

	void SetPosition(const glm::vec3& position);

private:
	void SetDirty();

	void UnsetDirty();

	glm::mat4 m_localToWorld{ 1.0f };
	glm::mat4 m_localRotation{ 1.0f };
	glm::mat4 m_localScale{ 1.0f };
	DirtyFlag m_dirtyFlag{ DirtyFlag::UNSET };
};
}
