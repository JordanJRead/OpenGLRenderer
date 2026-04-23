#include "shaders/shadermesh.h"
#include "vertexarray.h"
#include "transform.h"
#include "texturetypes.h"
#include "mesh.h"
#include "model.h"
#include "framebuffer.h"
#include <array>
#include "texture2d.h"
#include "glad/glad.h"

ShaderMesh::ShaderMesh(std::string_view vertPath, std::string_view fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	for (int i{ 0 }; i < (int)TextureTypes::max; ++i) {
		setInt(TextureTypes::names[i] + "Texture", i);
	}
}

void ShaderMesh::render(const Mesh& mesh, const Model& parentModel, int objectSceneIndex, const Framebuffer* framebuffer, const Transform& transform) const {
	bind();
	if (framebuffer) {
		framebuffer->bind();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	setMatrix4("model", transform.getModelMatrix());
	setVector3("diffuseColour", mesh.getMaterial().mDiffuseColour);
	setVector3("specularColour", mesh.getMaterial().mSpecularColour);
	setFloat("specularExponent", mesh.getMaterial().mSpecularExponent);
	setInt("sceneIndex", objectSceneIndex);

	std::array<const Texture2D*, TextureTypes::max> textures{ mesh.getTextures(parentModel) };
	for (int i = 0; i < textures.size(); ++i) {
		textures[i]->bind(i);
	}

	mesh.getVertexArray().bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh.getVertexArray().getIndexCount(), GL_UNSIGNED_INT, nullptr);
}