#ifndef SHADER_POINT_LIGHT
#define SHADER_POINT_LIGHT

#include "shaderi.h"
#include <string>
#include "pointlight.h"
#include "vertexarray.h"

class ShaderPointLight : public ShaderI {
public:
	ShaderPointLight(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {

	}

	void render(const VertexArray& vertexArray, const PointLight& pointLight, const glm::vec3& scale) const {
		vertexArray.bind();
		setVector3("position", pointLight.position);;
		setVector3("scale", scale);
		setVector3("colour", pointLight.colour);
		glDrawElements(GL_TRIANGLES, vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
	}

	void setMatrices(const Camera& camera, const Transform& transform, int screenWidth, int screenHeight) const {
		bind();
		setMatrix4("model", transform.getModelMatrix());
		setMatrix4("view", camera.getViewMatrix());
		setMatrix4("projection", camera.getProjectionMatrix(screenWidth, screenHeight));
	}
};

#endif