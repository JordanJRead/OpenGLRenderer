#include "shaders/shaderi.h"
#include "shaders/shaderdeferred.h"

ShaderDeferred::ShaderDeferred(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	setInt("worldPosBuffer", 0);
	setInt("normalBuffer", 1);
	setInt("diffuseBuffer", 2);
	setInt("specularDataBuffer", 3);
}

void ShaderDeferred::render(const VertexArrayScreen& screenVertexArray, const Framebuffer& geometryBuffers, const DirectionalLight& directionalLight, const glm::vec3& ambientLightColour) {
	bind();
	geometryBuffers.bindColourTexture(0, 0);
	geometryBuffers.bindColourTexture(1, 1);
	geometryBuffers.bindColourTexture(2, 2);
	geometryBuffers.bindColourTexture(3, 3);
	setVector3("directionalLight.dirTo", directionalLight.mDirTo);
	setVector3("directionalLight.colour", directionalLight.mColour);
	setVector3("ambientLightColour", ambientLightColour);
	screenVertexArray.bind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawElements(GL_TRIANGLES, (GLsizei)screenVertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}