#include "shaders/shaderi.h"
#include "shaders/shaderdeferred.h"

ShaderDeferred::ShaderDeferred(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	setInt("worldPosBuffer", 0);
	setInt("normalBuffer", 1);
	setInt("diffuseBuffer", 2);
	setInt("specularDataBuffer", 3);
}

void ShaderDeferred::render(const VertexArrayScreen& screenVertexArray, const Framebuffer& geometryBuffers, const glm::vec3& cameraPos) {
	bind();
	geometryBuffers.bindColourTexture(0, 0);
	geometryBuffers.bindColourTexture(1, 1);
	geometryBuffers.bindColourTexture(2, 2);
	geometryBuffers.bindColourTexture(3, 3);
	setVector3("directionalLight.dirTo", glm::normalize(glm::vec3{ 1, 0.5, 0 }));
	setVector3("directionalLight.colour", glm::vec3{ 0, 0, 0 });
	setVector3("ambientLightColour", glm::vec3{ 0.2, 0.2, 0.2 });
	setVector3("cameraPos", cameraPos);
	screenVertexArray.bind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawElements(GL_TRIANGLES, screenVertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}