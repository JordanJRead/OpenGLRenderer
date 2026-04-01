#include "shaders/shaderobject.h"
#include "vertexarray.h"

ShaderObject::ShaderObject(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	for (int i{ 0 }; i < (int)TextureTypes::max; ++i) {
		setInt(TextureTypes::names[i] + "Texture", i);
	}
}

void ShaderObject::setMatrices(const Camera& camera, const Transform& transform, int screenWidth, int screenHeight) const {
	bind();
	setMatrix4("model", transform.getModelMatrix());
	setMatrix4("view", camera.getViewMatrix());
	setMatrix4("projection", camera.getProjectionMatrix(screenWidth, screenHeight));
}

void ShaderObject::render(const VertexArray& vertexArray, const std::array<const Texture2D*, TextureTypes::max> textures, const glm::vec3& diffuseColour) const {
	bind();
	setVector3("diffuseColour", diffuseColour);
	vertexArray.bind();
	for (int i = 0; i < textures.size(); ++i) {
		textures[i]->bind(i);
	}
	glDrawElements(GL_TRIANGLES, vertexArray.getIndexCount(), GL_UNSIGNED_INT, nullptr);
}