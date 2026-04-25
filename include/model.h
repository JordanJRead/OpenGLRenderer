#ifndef MODEL_H
#define MODEL_H

#include <string_view>
#include <vector>
#include "mesh.h"
#include "glad/glad.h"
#include "component.h"
#include <span>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct aiMesh;
struct aiNode;
struct aiScene;
class Mesh;
namespace TextureTypes {
	enum Type;
}

class Model : public Component {
	friend class Mesh;
public:
	static ComponentTypes::Type getComponentType() { return ComponentTypes::model; }

	Model(std::string_view objPath);
	static std::unique_ptr<Component> fromJSON(const json& json);
	const std::vector<Mesh>& getMeshes() const { return mMeshes; }
	void renderUIProperties() override {}
	const std::span<const Mesh> getMeshes();
	json toJSON() override;

private:
	std::vector<Mesh> mMeshes;
	std::vector<Texture2D> mTextures;
	std::string mDirectory;
	std::string mObjPath;

	void processNode(aiNode* mesh, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	size_t addTexture(std::string_view imagePath);
	const Texture2D& getTexture(size_t index, TextureTypes::Type textureType) const;
};

#endif