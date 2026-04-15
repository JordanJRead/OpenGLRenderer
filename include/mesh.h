#ifndef MESH_H
#define MESH_H

#include "material.h"
#include "vertexarray.h"
#include <vector>
#include <array>
#include "texture2d.h"
#include "texturetypes.h"
#include "transform.h"

class Model;

class Mesh {
public:
	Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& indices, const std::vector<int>& vertexLayout, const Material& material, bool hasTexCoords);
	const Material& getMaterial() const { return mMaterial; }
	bool hasTexCoords() const { return mHasTexCoords; }
	const VertexArray& getVertexArray() const { return mVertexArray; }
	std::array<const Texture2D*, TextureTypes::max> getTextures(const Model& parentModel) const;

private:
	Material mMaterial;
	VertexArray mVertexArray;
	bool mHasTexCoords;
};

#endif