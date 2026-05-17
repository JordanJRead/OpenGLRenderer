#include "pointlight.h"
#include "vertexarray.h"
#include "shaders/shaderpointlight.h"
#include "transform.h"
#include "camera.h"
#include "framebuffer.h"
#include "sceneobject.h"

ShaderPointLight::ShaderPointLight(std::string_view vertPath, std::string_view fragPath) : ShaderI{ vertPath, fragPath } {

}

void ShaderPointLight::render(const SceneObject* const object, bool highlight, const VertexArray& vertexArray, const Framebuffer* framebuffer, const glm::vec3& position, const glm::vec3& lightColour) const {
	bind();
	Framebuffer::bind(framebuffer);

	setVector3("position", position);;
	setVector3("colour", lightColour);
	setFloat("objectPtrFirst", *((float*)(&object)));
	setFloat("objectPtrSecond", *(((float*)(&object)) + 1));
	setBool("highlight", highlight);

	vertexArray.bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}