#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glad/glad.h"
#include "OpenGLObjects/FBO.h"
#include "OpenGLObjects/TEX.h"
#include "OpenGLObjects/RBO.h"
#include <iostream>
#include "texture2d.h"
#include <vector>
#include <array>

class Framebuffer {
public:
	/// <summary>
	/// Creates a framebuffer with one colour attachment and one depth/stencil attachment
	/// </summary>
	/// <param name="width">The width of the framebuffer</param>
	/// <param name="height">The height of the framebuffer</param>
	/// <param name="formats">The internal pixel formats of each colour texture (GL_RGB, GL_RGBA, etc.)</param>
	Framebuffer(int width, int height, std::vector<GLenum> formats)
		: mWidth{ width }
		, mHeight{ height }
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		const size_t colourTextureCount = formats.size();
		std::vector<GLenum> attachments;
		for (int i{ 0 }; i < colourTextureCount; ++i) {
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
			mColourTextures.emplace_back(width, height, formats[i]);
			mColourTextures[i].bind(0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mColourTextures[i], 0);
		}
		if (attachments.size() == 0) {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else if (attachments.size() > 1) {
			glDrawBuffers((GLsizei)attachments.size(), attachments.data());
		}

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
	void bindColourTexture(int colourTextureIndex, int unit) const {
		mColourTextures[colourTextureIndex].bind(unit);
	}

	const Texture2D& getColourTexture(int colourTextureIndex) const {
		return mColourTextures[colourTextureIndex];
	}
	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }

	std::array<float, 4> samplePixel(int x, int y, int colourTextureIndex) const {
		bind();
		glReadBuffer(GL_COLOR_ATTACHMENT0 + colourTextureIndex);
		std::array<float, 4> data;
		glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, data.data());
		glReadBuffer(GL_BACK);
		return data;
	}

private:
	FBO mFBO;
	std::vector<Texture2D> mColourTextures;
	RBO mDepthStencilRenderbuffer;
	int mWidth;
	int mHeight;
};

#endif