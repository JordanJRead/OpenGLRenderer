#include <string_view>
#include "texture2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
#include <iostream>

Texture2D::Texture2D(int width, int height, GLenum format, GLenum wrapOption, GLenum filterOption)
	: mFormat{ format }
{
	glBindTexture(GL_TEXTURE_2D, mTEX);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterOption);

	glTexImage2D(GL_TEXTURE_2D, 0, mFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(std::string_view filePath, bool srgba, GLenum wrapOption, GLenum filterOption)
	: mFilePath{ filePath }
{
	stbi_set_flip_vertically_on_load(false);
	int width, height, sourceChannels;
	unsigned char* data = stbi_load(filePath.data(), &width, &height, &sourceChannels, 4);

	if (!data) {
		std::cerr << "Error loading file " << filePath << ". Reason: " << stbi_failure_reason() << "\n";
	}

	glBindTexture(GL_TEXTURE_2D, mTEX);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterOption);

	mFormat = srgba ? GL_SRGB8_ALPHA8 : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, mFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void Texture2D::bind(unsigned int unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, mTEX);
}

const std::string& Texture2D::getFilePath() {
	return mFilePath;
}