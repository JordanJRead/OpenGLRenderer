#include "sceneobject.h"
#include "assimp/importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <vector>

SceneObject::SceneObject(const std::string& objPath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(objPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	processNode(scene->mRootNode, scene);
}

void SceneObject::processNode(aiNode* node, const aiScene* scene) {
	for (int i{ 0 }; i < node->mNumMeshes; ++i) {
		processMesh(scene->mMeshes[node->mMeshes[i]]);
	}

	for (int i{ 0 }; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

void SceneObject::processMesh(aiMesh* mesh) {
	// Vertex data
	std::vector<float> vertexData;
	std::vector<unsigned int> indices;
	std::vector<int> vertexLayout{ 3, 3, 2 };

	for (int i{ 0 }; i < mesh->mNumVertices; ++i) {
		vertexData.push_back(mesh->mVertices[i].x);
		vertexData.push_back(mesh->mVertices[i].y);
		vertexData.push_back(mesh->mVertices[i].z);

		vertexData.push_back(mesh->mNormals[i].x);
		vertexData.push_back(mesh->mNormals[i].y);
		vertexData.push_back(mesh->mNormals[i].z);

		vertexData.push_back(mesh->mTextureCoords[0][i].x);
		vertexData.push_back(mesh->mTextureCoords[0][i].y);
	}

	for (int i{ 0 }; i < mesh->mNumFaces; ++i) {
		for (int j{ 0 }; j < mesh->mFaces[i].mNumIndices; ++j) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	// Material data
	Material material;

	mMeshes.emplace_back(vertexData, indices, vertexLayout, material);
}
