#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <string>
#include <string_view>
#include <vector>
#include "mesh.h"
#include "texturetypes.h"
#include "transform.h"
#include "shaderi.h"
#include <array>
#include "framebuffer.h"
#include "glad/glad.h"

struct aiMesh;
struct aiNode;
struct aiScene;
class Mesh;
class ShaderI;
class Camera;

class SceneObject {
public:
	SceneObject(const std::string& objPath, const Transform& transform);
	~SceneObject();
	const std::vector<Mesh>& getMeshes() const { return mMeshes; }

	void render(int screenWidth, int screenHeight, const Camera& camera, const Framebuffer* const targetFramebuffer);

private:
	std::vector<Mesh> mMeshes;
	std::vector<Texture2D> mTextures;
	Transform mTransform;
	std::string mDirectory;

	void processNode(aiNode* mesh, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	size_t addTexture(const std::string& imagePath);
	const Texture2D& getTexture(int index, TextureTypes::Type textureType);

	static inline ShaderI* mDefaultShader;
	static inline ShaderI* mDefaultNoTexShader;
};

#endif