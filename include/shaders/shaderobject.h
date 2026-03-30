#ifndef OBJECT_SHADER_I
#define OBJECT_SHADER_I

#include "shaderi.h"
#include <string>
#include "camera.h"
#include "transform.h"
#include "texturetypes.h"

class ShaderObject : public ShaderI {
public:
	ShaderObject(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
		bind();
		for (int i{ 0 }; i < (int)TextureTypes::max; ++i) {
			setInt(TextureTypes::names[i] + "Texture", i);
		}
	}

	void setMatrices(const Camera& camera, const Transform& transform, int screenWidth, int screenHeight) const {
		bind();
		setMatrix4("model", transform.getModelMatrix());
		setMatrix4("view", camera.getViewMatrix());
		setMatrix4("projection", camera.getProjectionMatrix(screenWidth, screenHeight));
	}

	void setObjectColour(const glm::vec3& colour) const {
		bind();
		setVector3("diffuseColour", colour);
	}

protected:
};

#endif