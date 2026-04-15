#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <string_view>
#include <vector>
#include "mesh.h"
#include "texturetypes.h"
#include <array>
#include "framebuffer.h"
#include "glad/glad.h"
#include "shaders/shaderobject.h"
#include "component.h"
#include <span>

struct aiMesh;
struct aiNode;
struct aiScene;
class Mesh;
class ShaderI;
class Camera;

class Model : public Component {
	friend class Mesh;
public:
	static ComponentTypes::Type getComponentType() { return ComponentTypes::model; }

	Model(const std::string& objPath);
	const std::vector<Mesh>& getMeshes() const { return mMeshes; }
	void renderUIProperties() override {}
	const std::span<const Mesh> getMeshes();

private:
	std::vector<Mesh> mMeshes;
	std::vector<Texture2D> mTextures;
	std::string mDirectory;

	void processNode(aiNode* mesh, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	size_t addTexture(const std::string& imagePath);
	const Texture2D& getTexture(size_t index, TextureTypes::Type textureType) const;
};

#endif