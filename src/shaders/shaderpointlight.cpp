#include "pointlight.h"
#include "vertexarray.h"
#include "shaders/shaderpointlight.h"
#include "transform.h"
#include "camera.h"
#include "framebuffer.h"

ShaderPointLight::ShaderPointLight(std::string_view vertPath, std::string_view fragPath) : ShaderI{ vertPath, fragPath } {

}

void ShaderPointLight::render(const VertexArray& vertexArray, const Framebuffer* framebuffer, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& lightColour) const {
	bind();
	if (framebuffer) {
		framebuffer->bind();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	setVector3("position", position);;
	setVector3("scale", scale);
	setVector3("colour", lightColour);

	vertexArray.bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}