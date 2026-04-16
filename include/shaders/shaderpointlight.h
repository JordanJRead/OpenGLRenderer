#ifndef SHADER_POINT_LIGHT
#define SHADER_POINT_LIGHT

#include "shaders/shaderi.h"

class Camera;
class VertexArray;
class PointLight;
class Transform;
class Framebuffer;

class ShaderPointLight : public ShaderI {
public:
	ShaderPointLight(std::string_view vertPath, std::string_view fragPath);
	void render(const VertexArray& vertexArray, const Framebuffer* framebuffer, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& lightColour) const;
};

#endif