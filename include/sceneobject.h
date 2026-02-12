#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <string>
#include <string_view>
#include <vector>
#include "mesh.h"
#include "texture.h"

class aiMesh;
class aiNode;
class aiScene;

class SceneObject {
public:
	SceneObject(const std::string& objectDirectory);
	const std::vector<Mesh>& getMeshes() const { return mMeshes; }

private:
	std::vector<Mesh> mMeshes;
	std::vector<Texture> mTextures;

	void processNode(aiNode* mesh, const aiScene* scene);
	void processMesh(aiMesh* mesh);
};

#endif