#ifndef OBJECT_SHADER_I
#define OBJECT_SHADER_I

#include "shaderi.h"

class Transform;
class Mesh;
class Framebuffer;
class Model;
class SceneObject;

class ShaderMesh : public ShaderI {
public:
	ShaderMesh(std::string_view vertPath, std::string_view fragPath);
	void render(const Mesh& mesh, const Model& parentModel, const SceneObject* const object, bool highlight, const Framebuffer* framebuffer, const Transform& transform) const;

protected:
};

#endif