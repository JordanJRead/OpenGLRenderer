#include "sceneobject.h"
#include "assimp/importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <vector>
#include "shaderi.h"
#include "camera.h"

SceneObject::SceneObject(const std::string& objPath, const Transform& transform)
	: mTransform{transform}
	, mDirectory{ objPath }
	{
	mDirectory.resize(mDirectory.rfind("/"));
	if (mDefaultDiffuseTexture == nullptr) {
		mDefaultDiffuseTexture = new Texture{ "assets/images/white.png" };
		mDefaultSpecularTexture = new Texture{ "assets/images/black.png" };
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(objPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	processNode(scene->mRootNode, scene);
}

SceneObject::~SceneObject() {
	delete mDefaultDiffuseTexture;
	delete mDefaultSpecularTexture;
	mDefaultDiffuseTexture = nullptr;
	mDefaultSpecularTexture = nullptr;
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
	std::vector<int> vertexLayout{ 3, 3, 2 };

	for (unsigned int i{ 0 }; i < mesh->mNumVertices; ++i) {
		vertexData.push_back(mesh->mVertices[i].x);
		vertexData.push_back(mesh->mVertices[i].y);
		vertexData.push_back(mesh->mVertices[i].z);

		vertexData.push_back(mesh->mNormals[i].x);
		vertexData.push_back(mesh->mNormals[i].y);
		vertexData.push_back(mesh->mNormals[i].z);

		vertexData.push_back(mesh->mTextureCoords[0][i].x);
		vertexData.push_back(mesh->mTextureCoords[0][i].y);
	}

	for (unsigned int i{ 0 }; i < mesh->mNumFaces; ++i) {
		for (unsigned int j{ 0 }; j < mesh->mFaces[i].mNumIndices; ++j) {
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	// Material data
	Material material;
	material.mDiffuseTextureIndex = -1;
	material.mSpecularTextureIndex = -1;

	const aiMaterial* aiMaterial{ scene->mMaterials[mesh->mMaterialIndex] };
	if (aiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString diffusePath;
		aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffusePath);
		material.mDiffuseTextureIndex = addTexture(diffusePath.C_Str());
	}
	if (aiMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {
		aiString specularPath;
		aiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &specularPath);
		material.mDiffuseTextureIndex = addTexture(specularPath.C_Str());
	}

	aiColor3D diffuseColour;
	aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColour);
	aiColor3D specularColour;
	aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColour);

	material.mDiffuseColour = glm::vec3{ diffuseColour.r, diffuseColour.g, diffuseColour.b };
	material.mSpecularColour = glm::vec3{ specularColour.r, specularColour.g, specularColour.b };

	mMeshes.emplace_back(vertexData, indices, vertexLayout, material);
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

void SceneObject::render(int screenWidth, int screenHeight, const ShaderI& shader, const Camera& camera) {
	shader.use();
	shader.setMatrix4("model", mTransform.getModelMatrix());
	shader.setMatrix4("view", camera.getViewMatrix());
	shader.setMatrix4("projection", camera.getProjectionMatrix(screenWidth, screenHeight));
	shader.setInt("diffuseTexture", 0);
	
	for (const Mesh& mesh : mMeshes) {
		shader.setVector3("diffuseColour", mesh.getMaterial().mDiffuseColour);
		getTexture(mesh.getMaterial().mDiffuseTextureIndex, true).use(0);
		mesh.useVertexArray();
		glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}

const Texture& SceneObject::getTexture(int index, bool isDiffuse) {
	if (index >= mTextures.size() || index < 0) {
		if (isDiffuse) {
			return *mDefaultDiffuseTexture;
		}
		else {
			return *mDefaultSpecularTexture;
		}
	}
	return mTextures[index];
}