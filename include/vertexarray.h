#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <vector>
#include <glad/glad.h>
#include <OpenGLObjects/BUF.h>
#include <OpenGLObjects/VAO.h>
#include <string>
#include "assimp/importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class VertexArray {
public:
	/// <summary>
	/// Creates a vertex array object
	/// </summary>
	/// <param name="vertexData">The raw vertex data</param>
	/// <param name="vertexIndices">The vertex indices</param>
	/// <param name="vertexLayout">A vector that specifies how many float components are in each attribute</param>
	void create(const std::vector<float>& vertexData, const std::vector<unsigned int>& vertexIndices, const std::vector<int>& vertexLayout);

	/// <summary>
	/// Creates a vertex array object with only positional data from an obj file
	/// </summary>
	/// <param name="objPath"></param>
	void create(const std::string& objPath) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(objPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		std::vector<float> vertexData;
		std::vector<unsigned int> vertexIndices;
		std::vector<int> vertexLayout{ 3 };

		std::vector<aiNode*> nodes;
		nodes.push_back(scene->mRootNode);

		while (nodes.size() > 0) {
			aiNode* currentNode{ nodes.back() };
			nodes.pop_back();

			for (unsigned int meshI{ 0 }; meshI < currentNode->mNumMeshes; ++meshI) {
				aiMesh* currentMesh{ scene->mMeshes[currentNode->mMeshes[meshI]] };

				for (unsigned int vertexI{ 0 }; vertexI < currentMesh->mNumVertices; ++vertexI) {
					vertexData.push_back(currentMesh->mVertices[vertexI].x);
					vertexData.push_back(currentMesh->mVertices[vertexI].y);
					vertexData.push_back(currentMesh->mVertices[vertexI].z);
				}

				for (unsigned int faceI{ 0 }; faceI < currentMesh->mNumFaces; ++faceI) {
					for (unsigned int indexI{ 0 }; indexI < currentMesh->mFaces[faceI].mNumIndices; ++indexI) {
						vertexIndices.push_back(currentMesh->mFaces[faceI].mIndices[indexI]);
					}
				}
			}

			for (unsigned int childI{ 0 }; childI < currentNode->mNumChildren; ++childI) {
				nodes.push_back(currentNode->mChildren[childI]);
			}

		}
		create(vertexData, vertexIndices, vertexLayout);
	}

	/// <summary>
	/// Binds this vertex array object
	/// </summary>
	void bind() const { glBindVertexArray(mVAO); }

	/// <summary>
	/// Gets the number of indices
	/// </summary>
	size_t getIndexCount() const { return mIndexCount; }

private:
	VAO mVAO;
	BUF mVBO;
	BUF mEBO;
	size_t mIndexCount{};
};

#endif