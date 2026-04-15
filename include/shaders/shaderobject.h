#ifndef OBJECT_SHADER_I
#define OBJECT_SHADER_I

#include "shaderi.h"
#include "transform.h"
#include "texturetypes.h"
#include "mesh.h"
#include "framebuffer.h"

class Transform;
class Material;

class ShaderObject : public ShaderI {
public:
	ShaderObject(const std::string& vertPath, const std::string& fragPath);
	void render(const Mesh& mesh, const Model& parentModel, const Framebuffer* framebuffer, const Transform& transform) const;

protected:
};

#endif