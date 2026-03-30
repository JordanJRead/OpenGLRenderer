#include "sceneobject.h"
#include "assimp/importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <vector>
#include "camera.h"
#include "texturetypes.h"

SceneObject::SceneObject(const std::string& objPath, const Transform& transform)
	: mTransform{transform}
	, mDirectory{ objPath }
	{
	mDirectory.resize(mDirectory.rfind("/"));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(objPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	processNode(scene->mRootNode, scene);
}

void SceneObject::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i{ 0 }; i < node->mNumMeshes; ++i) {
		processMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (unsigned int i{ 0 }; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

void SceneObject::processMesh(aiMesh* mesh, const aiScene* scene) {
	// Vertex data
	std::vector<float> vertexData;
	std::vector<unsigned int> indices;
	bool hasTexCoords{ mesh->HasTextureCoords(0) };

	for (unsigned int i{ 0 }; i < mesh->mNumVertices; ++i) {
		vertexData.push_back(mesh->mVertices[i].x);
		vertexData.push_back(mesh->mVertices[i].y);
		vertexData.push_back(mesh->mVertices[i].z);

		vertexData.push_back(mesh->mNormals[i].x);
		vertexData.push_back(mesh->mNormals[i].y);
		vertexData.push_back(mesh->mNormals[i].z);

		vertexData.push_back(mesh->mTangents[i].x);
		vertexData.push_back(mesh->mTangents[i].y);
		vertexData.push_back(mesh->mTangents[i].z);

		if (hasTexCoords) {
			vertexData.push_back(mesh->mTextureCoords[0][i].x);
			vertexData.push_back(mesh->mTextureCoords[0][i].y);
		}
	}
	std::vector<int> vertexLayout{ 3, 3, 3, 2 };
	if (!hasTexCoords) {
		vertexLayout.pop_back();
	}

	for (unsigned int i{ 0 }; i < mesh->mNumFaces; ++i) {
		for (unsigned int j{ 0 }; j < mesh->mFaces[i].mNumIndices; ++j) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	// Material data
	Material material{};
	material.mTextureIndices.fill(-1);

	const aiMaterial* aiMaterial{ scene->mMaterials[mesh->mMaterialIndex] };
	for (int i{ 0 }; i < (int)TextureTypes::Type::max; ++i) {
		if (aiMaterial->GetTextureCount(TextureTypes::assimpTypes[i])) {
			aiString path;
			aiMaterial->GetTexture(TextureTypes::assimpTypes[i], 0, &path);
			material.mTextureIndices[i] = addTexture(path.C_Str());
		}
	}

	aiColor3D diffuseColour;
	aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColour);
	aiColor3D specularColour;
	aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColour);

	material.mDiffuseColour = glm::vec3{ diffuseColour.r, diffuseColour.g, diffuseColour.b };
	material.mSpecularColour = glm::vec3{ specularColour.r, specularColour.g, specularColour.b };

	mMeshes.emplace_back(vertexData, indices, vertexLayout, material, hasTexCoords);
}

/// <summary>
/// Adds a new texture if the texture does not already exist
/// </summary>
/// <returns>The index to the texture</returns>
size_t SceneObject::addTexture(const std::string& filePath) {
	for (int i{ 0 }; i < mTextures.size(); ++i) {
		if (mTextures[i].getFilePath() == filePath) {
			return i;
		}
	}
	mTextures.emplace_back(mDirectory + "/" + filePath);
	return mTextures.size() - 1;
}

void SceneObject::render(int screenWidth, int screenHeight, const Camera& camera, const ShaderObject& shader, const Framebuffer* const targetFramebuffer) {
	if (targetFramebuffer) {
		targetFramebuffer->bind();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	shader.setMatrices(camera, mTransform, screenWidth, screenHeight);
	
	for (const Mesh& mesh : mMeshes) {
		shader.setObjectColour(mesh.getMaterial().mDiffuseColour);
		for (int i{ 0 }; i < (int)TextureTypes::Type::max; ++i) {
			getTexture(mesh.getMaterial().mTextureIndices[i], (TextureTypes::Type)i).bind(i);
		}
		mesh.bindVertexArray();
		glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}

const Texture2D& SceneObject::getTexture(int index, TextureTypes::Type textureType) {
	if (index >= mTextures.size() || index < 0) {
		return *TextureTypes::defaultImages[(int)textureType];
	}
	return mTextures[index];
}