#ifndef SHADER_POST_PROCESS
#define SHADER_POST_PROCESS

#include "shaderi.h"

class Texture2D;
class Framebuffer;
class VertexArrayScreen;

class ShaderPostProcess : public ShaderI {
public:
	ShaderPostProcess(std::string_view vertPath, std::string_view fragPath);
	void render(const VertexArrayScreen& screenVertexArray, const Framebuffer* targetFramebuffer, const Texture2D& sourceTexture);
};

#endif