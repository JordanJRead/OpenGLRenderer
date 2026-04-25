#include "model.h"
#include "assimp/importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "camera.h"
#include "texturetypes.h"
#include <stdexcept>
#include <string>

Model::Model(std::string_view objPath)
	: mDirectory{ objPath }
	, mObjPath{ objPath }
	, Component{ ComponentTypes::model }
	{
	mDirectory.resize(mDirectory.rfind("/"));

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(objPath.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

	processNode(scene->mRootNode, scene);
}

std::unique_ptr<Component> Model::fromJSON(const JSON& json) {
	return std::make_unique<Model>(json.at("objPath"));
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i{ 0 }; i < node->mNumMeshes; ++i) {
		processMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (unsigned int i{ 0 }; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
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

		if (mesh->HasTangentsAndBitangents()) {
			vertexData.push_back(mesh->mTangents[i].x);
			vertexData.push_back(mesh->mTangents[i].y);
			vertexData.push_back(mesh->mTangents[i].z);

			vertexData.push_back(mesh->mBitangents[i].x);
			vertexData.push_back(mesh->mBitangents[i].y);
			vertexData.push_back(mesh->mBitangents[i].z);
		}
		else {
			vertexData.push_back(1);
			vertexData.push_back(0);
			vertexData.push_back(0);

			vertexData.push_back(0);
			vertexData.push_back(1);
			vertexData.push_back(0);
		}

		if (hasTexCoords) {
			vertexData.push_back(mesh->mTextureCoords[0][i].x);
			vertexData.push_back(mesh->mTextureCoords[0][i].y);
		}
	}
	std::vector<int> vertexLayout{ 3, 3, 3, 3, 2 };
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
	float mSpecularExponent;
	if (aiMaterial->Get(AI_MATKEY_SHININESS, specularColour) != AI_SUCCESS || true) {
		mSpecularExponent = 100000.0f;
	}

	material.mDiffuseColour = glm::vec3{ diffuseColour.r, diffuseColour.g, diffuseColour.b };
	material.mSpecularColour = glm::vec3{ specularColour.r, specularColour.g, specularColour.b };
	material.mSpecularExponent = mSpecularExponent;

	mMeshes.emplace_back(vertexData, indices, vertexLayout, material, hasTexCoords);
}

/// <summary>
/// Adds a new texture if the texture does not already exist
/// </summary>
/// <returns>The index to the texture</returns>
size_t Model::addTexture(std::string_view filePath) {
	for (int i{ 0 }; i < mTextures.size(); ++i) {
		if (mTextures[i].getFilePath() == filePath) {
			return i;
		}
	}
	std::string fullFilePath{ mDirectory };
	fullFilePath += "/";
	fullFilePath += filePath;
	mTextures.emplace_back(fullFilePath);
	return mTextures.size() - 1;
}

const Texture2D& Model::getTexture(size_t index, TextureTypes::Type textureType) const {
	if (index >= mTextures.size() || index < 0) {
		return *TextureTypes::defaultImages[(int)textureType];
	}
	return mTextures[index];
}

const std::span<const Mesh> Model::getMeshes() {
	return mMeshes;
}

JSON Model::toJSON() {
	JSON json;
	json["objPath"] = mObjPath;
	return json;
}