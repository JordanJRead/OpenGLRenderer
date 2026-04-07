#ifndef OBJECT_SHADER_I
#define OBJECT_SHADER_I

#include "shaderi.h"
#include <string>
#include "camera.h"
#include "transform.h"
#include "texturetypes.h"

class ShaderObject : public ShaderI {
public:
	ShaderObject(const std::string& vertPath, const std::string& fragPath);
	void setMatrices(const Camera& camera, const Transform& transform, int screenWidth, int screenHeight) const;
	void render(const VertexArray& vertexArray, const std::array<const Texture2D*, TextureTypes::max>, const glm::vec3& diffuseColour) const;
	void setNormalMapping(bool value) { bind(); setBool("doNormalMapping", value); }

protected:
};

#endif