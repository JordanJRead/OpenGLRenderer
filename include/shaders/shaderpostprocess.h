#ifndef SHADER_POST_PROCESS
#define SHADER_POST_PROCESS

#include "shaderi.h"
#include <string>
#include "texture2d.h"

class ShaderPostProcess : public ShaderI {
public:
	ShaderPostProcess(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
		bind();
		setInt("source", 0);
	}

	void setSourceTexture(const Texture2D& sourceTexture) {
		bind();
		sourceTexture.bind(0);
	}
};

#endif