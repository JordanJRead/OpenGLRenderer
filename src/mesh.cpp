#include "mesh.h"

Mesh::Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& indices, const std::vector<int>& vertexLayout, const Material& material, bool hasTexCoords)
	: mMaterial{ material }
	, mHasTexCoords{ hasTexCoords }
{
	mVertexArray.create(vertexData, indices, vertexLayout);
}