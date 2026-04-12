#ifndef OBJECT_SHADER_I
#define OBJECT_SHADER_I

#include "shaderi.h"
#include <string>
#include "camera.h"
#include "transform.h"
#include "texturetypes.h"
#include "material.h"

class ShaderObject : public ShaderI {
public:
	ShaderObject(const std::string& vertPath, const std::string& fragPath);
	void setModelMatrix(const Transform& transform) const;
	void render(const VertexArray& vertexArray, const std::array<const Texture2D*, TextureTypes::max>, const Material& material) const;

protected:
};

#endif