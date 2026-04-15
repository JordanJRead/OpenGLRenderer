#include "mesh.h"
#include "model.h"

Mesh::Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& indices, const std::vector<int>& vertexLayout, const Material& material, bool hasTexCoords)
	: mMaterial{ material }
	, mHasTexCoords{ hasTexCoords }
{
	mVertexArray.create(vertexData, indices, vertexLayout);
}

std::array<const Texture2D*, TextureTypes::max> Mesh::getTextures(const Model& parentModel) const {
	std::array<const Texture2D*, TextureTypes::max> textures;

	for (int i = 0; i < TextureTypes::max; ++i) {
		textures[i] = &(parentModel.getTexture(mMaterial.mTextureIndices[i], (TextureTypes::Type)i));
	}
	return textures;
}