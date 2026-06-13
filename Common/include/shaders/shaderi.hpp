#ifndef SHADER_H
#define SHADER_H

#include <string_view>
#include "glm/glm.hpp"

class FramebufferI;
class VertexArray;

class ShaderI {

protected:
	unsigned int mID;
	ShaderI(const ShaderI&) = delete;
	ShaderI(ShaderI&&) = delete;

	ShaderI(std::string_view vertPath, std::string_view fragPath);
	~ShaderI();
	void bind() const;

	void setMatrix4(std::string_view name, const glm::mat4& mat4) const;
	void setVector3(std::string_view name, const glm::vec3& v) const;
	void setVector2(std::string_view name, const glm::vec2& v) const;
	void setFloat(std::string_view name, float f) const;
	void setUInt(std::string_view name, unsigned int n) const;
	void setInt(std::string_view name, int n) const;
	void setBool(std::string_view name, bool b) const;
};

#endif