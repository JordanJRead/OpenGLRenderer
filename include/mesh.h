#ifndef MESH_H
#define MESH_H

#include "material.h"
#include "vertexarray.h"
#include "sceneobject.h"
#include <vector>

class Mesh {
public:
	Mesh(const std::vector<float>& vertexData, const std::vector<unsigned int>& indices, const std::vector<int>& vertexLayout, const Material& material, bool hasTexCoords);
	const Material& getMaterial() const { return mMaterial; }
	void useVertexArray() const { mVertexArray.use(); }
	int getIndexCount() const { return mVertexArray.getIndexCount(); }
	bool hasTexCoords() const { return mHasTexCoords; }

private:
	Material mMaterial;
	VertexArray mVertexArray;
	bool mHasTexCoords;
};

#endif