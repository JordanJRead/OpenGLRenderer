#ifndef SHADER_DEFERRED_H
#define SHADER_DEFERRED_H

#include "shaderi.h"
#include <string>
#include "texture2d.h"
#include "framebuffer.h"

class ShaderDeferred : public ShaderI {
public:
	ShaderDeferred(const std::string& vertPath, const std::string& fragPath);
	void setPerFrameInfo(const Framebuffer& framebuffer);
};

#endif