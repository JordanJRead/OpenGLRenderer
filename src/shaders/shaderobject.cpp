#include "shaders/shaderobject.h"
#include "vertexarray.h"

ShaderObject::ShaderObject(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	for (int i{ 0 }; i < (int)TextureTypes::max; ++i) {
		setInt(TextureTypes::names[i] + "Texture", i);
	}
}

void ShaderObject::setModelMatrix(const Transform& transform) const {
	bind();
	setMatrix4("model", transform.getModelMatrix());
}

void ShaderObject::render(const VertexArray& vertexArray, const std::array<const Texture2D*, TextureTypes::max> textures, const Material& material) const {
	bind();
	setVector3("diffuseColour", material.mDiffuseColour);
	setVector3("specularColour", material.mSpecularColour);
	setFloat("specularExponent", material.mSpecularExponent);
	vertexArray.bind();
	for (int i = 0; i < textures.size(); ++i) {
		textures[i]->bind(i);
	}
	glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray.getIndexCount(), GL_UNSIGNED_INT, nullptr);
}