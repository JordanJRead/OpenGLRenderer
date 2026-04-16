#ifndef SHADER_DEFERRED_H
#define SHADER_DEFERRED_H

#include "shaderi.h"

class VertexArrayScreen;
class Framebuffer;
struct DirectionalLight;

class ShaderDeferred : public ShaderI {
public:
	ShaderDeferred(std::string_view vertPath, std::string_view fragPath);
	void render(const VertexArrayScreen& screenVertexArray, const Framebuffer* targetFramebuffer, const Framebuffer& geometryBuffers, const DirectionalLight& directionalLight, const glm::vec3& ambientLightColour);
};

#endif