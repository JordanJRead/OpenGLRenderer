#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glad/glad.h"
#include "OpenGLObjects/FBO.h"
#include "OpenGLObjects/TEX.h"
#include "OpenGLObjects/RBO.h"
#include <iostream>
#include "texture2d.h"

class Framebuffer {
public:
	/// <summary>
	/// Creates a framebuffer with one colour attachment and one depth/stencil attachment
	/// </summary>
	/// <param name="width">The width of the framebuffer</param>
	/// <param name="height">The height of the framebuffer</param>
	/// <param name="format">The internal pixel format of the frambuffer (GL_RGB, GL_RGBA, etc.)</param>
	Framebuffer(int width, int height, GLenum format)
		: mWidth{ width }
		, mHeight{ height }
		, mColourTexture{ width, height, format }
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColourTexture, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilRenderbuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "INCOMPLETE FRAMEBUFFER\n";
		}
	}

	/// <summary>
	/// Binds the framebuffer and sets the viewport size to fit
	/// </summary>
	void bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glViewport(0, 0, mWidth, mHeight);
	}

	/// <summary>
	/// Binds the colour texture to a specific texture unit
	/// </summary>
	/// <param name="unit"></param>
	void bindColourTexture(int unit) const {
		mColourTexture.bind(unit);
	}

	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }

private:
	FBO mFBO;
	Texture2D mColourTexture;
	RBO mDepthStencilRenderbuffer;
	int mWidth;
	int mHeight;
};

#endif