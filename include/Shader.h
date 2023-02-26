#pragma once

#include <string>

#include <GL/glew.h>

namespace diamond_engine {
class Shader {
public:
	static const size_t kMaxSourceLength = 0x200;

	Shader(GLenum type = GL_VERTEX_SHADER);

	void SetSource(const std::string& source);

	void Compile();

	bool Load(const std::string& file);

	std::string GetSource() const;

	bool IsCompiled() const;

	GLuint GetObject() const;

	~Shader();

private:
	GLuint m_object{ 0 };
	GLint m_compiled{ GL_FALSE };
};
}
