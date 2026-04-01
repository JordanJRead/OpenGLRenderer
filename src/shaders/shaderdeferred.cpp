#include "shaders/shaderi.h"
#include "shaders/shaderdeferred.h"

ShaderDeferred::ShaderDeferred(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	setInt("worldPosBuffer", 0);
	setInt("normalBuffer", 1);
	setInt("albedoBuffer", 2);
}

void ShaderDeferred::render(const VertexArrayScreen& screenVertexArray, const Framebuffer& source) {
	bind();
	source.bindColourTexture(0, 0);
	source.bindColourTexture(1, 1);
	source.bindColourTexture(2, 2);
	setVector3("directionalLight.dirTo", glm::vec3{ 0, 1, 0 });
	setVector3("directionalLight.colour", glm::vec3{ 1, 1, 1 });
	screenVertexArray.bind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawElements(GL_TRIANGLES, screenVertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}