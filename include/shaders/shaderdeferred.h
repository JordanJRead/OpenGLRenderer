#ifndef SHADER_DEFERRED_H
#define SHADER_DEFERRED_H

#include "shaderi.h"
#include <string>
#include "texture2d.h"
#include "framebuffer.h"
#include "vertexarrayscreen.h"

class ShaderDeferred : public ShaderI {
public:
	ShaderDeferred(const std::string& vertPath, const std::string& fragPath);
	void render(const VertexArrayScreen& screenVertexArray, const Framebuffer& source);
};

#endif