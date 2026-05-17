#include "shaders/shaderpostprocess.h"
#include "texture2d.h"
#include "framebuffer.h"
#include "vertexarrayscreen.h"
#include "glad/glad.h"

ShaderPostProcess::ShaderPostProcess(std::string_view vertPath, std::string_view fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	setInt("source", 0);
}

void ShaderPostProcess::render(const VertexArrayScreen& screenVertexArray, const Framebuffer* targetFramebuffer, const Texture2D& sourceTexture) {
	bind();
	Framebuffer::bind(targetFramebuffer);

	sourceTexture.bind(0);

	screenVertexArray.bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)screenVertexArray.getIndexCount(), GL_UNSIGNED_INT, 0);
}
