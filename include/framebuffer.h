#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glad/glad.h"
#include "OpenGLObjects/FBO.h"
#include "OpenGLObjects/TEX.h"
#include "OpenGLObjects/RBO.h"

template <GLint InternalFormat>
class Framebuffer {
public:
	Framebuffer(int width, int height)
		: mWidth{ width }
		, mHeight{ height }
	{
		mColourTexture.use(0);
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColourTexture, 0);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilRenderbuffer);
	}

	void use() {
		mFramebuffer.use();
	}

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }

private:
	FBO mFramebuffer;
	TEX mColourTexture;
	RBO mDepthStencilRenderbuffer;
	int mWidth;
	int mHeight;
};

#endif