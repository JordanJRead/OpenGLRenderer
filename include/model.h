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

	Model(const JSON& json);
	Model();
	const std::vector<Mesh>& getMeshes() const { return mMeshes; }
	const std::span<const Mesh> getMeshes();
	bool isValid() const { return mIsValid; }

private:
	std::vector<Mesh> mMeshes;
	std::vector<Texture2D> mTextures;
	std::string mDirectory;
	std::string mObjPath;
	bool mIsValid;

	void processNode(aiNode* mesh, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	size_t addTexture(std::string_view imagePath);
	const Texture2D& getTexture(size_t index, TextureTypes::Type textureType) const;

	void create(EditableProperties& properties) override;
};

#endif