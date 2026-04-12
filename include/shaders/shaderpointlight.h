#ifndef SHADER_POINT_LIGHT
#define SHADER_POINT_LIGHT

#include <string>
#include "shaders/shaderi.h"

class Camera;
class VertexArray;
class PointLight;
struct Transform;

class ShaderPointLight : public ShaderI {
public:
	ShaderPointLight(const std::string& vertPath, const std::string& fragPath);
	void render(const VertexArray& vertexArray, const PointLight& pointLight, const glm::vec3& scale) const;
	void setModelMatrix(const Camera& camera, const Transform& transform) const;
};

#endif