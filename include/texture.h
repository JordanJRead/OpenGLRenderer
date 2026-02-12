#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGLObjects/TEX.h"
#include <string>
#include <string_view>

class Texture {
public:
	Texture(std::string_view filePath);
	void use(unsigned int unit) const;

private:
	TEX mTEX;
	int mWidth;
	int mHeight;
	std::string mFilePath;
};

#endif