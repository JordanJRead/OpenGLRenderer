#include "shaders/shaderpostprocess.h"

ShaderPostProcess::ShaderPostProcess(const std::string& vertPath, const std::string& fragPath) : ShaderI{ vertPath, fragPath } {
	bind();
	setInt("source", 0);
}

void ShaderPostProcess::setSourceTexture(const Texture2D& sourceTexture) {
	bind();
	sourceTexture.bind(0);
}