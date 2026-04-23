#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGLObjects/TEX.h"
#include <string>
#include <string_view>

class Texture2D {
public:
	/// <summary>
	/// Creates an empty texture
	/// </summary>
	/// <param name="width">The width of the empty texture</param>
	/// <param name="height">The height of the empty texture</param>
	/// <param name="format">The internal format of the empty texture (GL_RGB, GL_RGBA, etc.)</param>
	/// <param name="wrapOption">The texture wrapping option</param>
	/// <param name="filterOption">The texture filtering option</param>
	Texture2D(int width, int height, GLenum format, GLenum wrapOption = GL_REPEAT, GLenum filterOption = GL_NEAREST);

	/// <summary>
	/// Loads an RGBA image from an image file
	/// </summary>
	/// <param name="filePath">The path of the image file to load</param>
	/// <param name="srgba">Whether the image should be in the sRGB colour space</param>
	/// <param name="wrapOption">The texture wrapping option</param>
	/// <param name="filterOption">The texture filtering option</param>
	Texture2D(std::string_view filePath, bool srgba = true, GLenum wrapOption = GL_REPEAT, GLenum filterOption = GL_NEAREST);

	/// <summary>
	/// Binds this texture to a specific texture unit
	/// </summary>
	/// <param name="unit">The texture unit to bind to (0, 1, etc.)</param>
	void bind(unsigned int unit) const;

	/// <summary>
	/// Gets the file path of the image this texture was loaded from. Returns the empty string if this texture was not loaded from an image file
	/// </summary>
	const std::string& getFilePath();

	GLenum getFormat() const { return mFormat; }

	operator unsigned int() {
		return mTEX;
	}

private:
	TEX mTEX;
	std::string mFilePath;
	GLenum mFormat;
};

#endif