#include <string>
#include "pointlight.h"
#include "vertexarray.h"
#include "shaders/shaderpointlight.h"
#include "transform.h"
#include "camera.h"

ShaderPointLight::ShaderPointLight(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {

}

void ShaderPointLight::render(const VertexArray& vertexArray, const PointLight& pointLight, const glm::vec3& scale) const {
	vertexArray.bind();
	setVector3("position", pointLight.position);;
	setVector3("scale", scale);
	setVector3("colour", pointLight.colour);
	glDrawElements(GL_TRIANGLES, vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}

void ShaderPointLight::setModelMatrix(const Camera& camera, const Transform& transform) const {
	bind();
	setMatrix4("model", transform.getModelMatrix());
	setMatrix4("view", camera.getViewMatrix());
	setMatrix4("projection", camera.getProjectionMatrix());
}