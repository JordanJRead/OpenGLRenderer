#include "pointlight.hpp"
#include "vertexarray.hpp"
#include "shaders/shaderpointlight.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include "framebuffer.hpp"
#include "sceneobject.hpp"

ShaderPointLight::ShaderPointLight(std::string_view vertPath, std::string_view fragPath) : ShaderI{ vertPath, fragPath } {

}

void ShaderPointLight::render(const SceneObject& object, bool highlight, const VertexArray& vertexArray, const Framebuffer* framebuffer, const glm::vec3& position, const glm::vec3& lightColour) const {
	bind();
	Framebuffer::bind(framebuffer);

	setVector3("position", position);;
	setVector3("colour", lightColour);
	const SceneObject* ptr{ &object };
	setFloat("objectPtrFirst", *((float*)(&ptr)));
	setFloat("objectPtrSecond", *(((float*)(&ptr)) + 1));
	setBool("highlight", highlight);

	vertexArray.bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}