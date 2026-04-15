#include <string>
#include "pointlight.h"
#include "vertexarray.h"
#include "shaders/shaderpointlight.h"
#include "transform.h"
#include "camera.h"

ShaderPointLight::ShaderPointLight(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {

}

void ShaderPointLight::render(const VertexArray& vertexArray, const glm::vec3& position, const glm::vec3& lightColour, const glm::vec3& scale) const {
	vertexArray.bind();
	setVector3("position", position);;
	setVector3("scale", scale);
	setVector3("colour", lightColour);
	glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}

void ShaderPointLight::setModelMatrix(const Camera& camera, const Transform& transform) const {
	bind();
	setMatrix4("model", transform.getModelMatrix());
	setMatrix4("view", camera.getViewMatrix());
	setMatrix4("projection", camera.getProjectionMatrix());
}